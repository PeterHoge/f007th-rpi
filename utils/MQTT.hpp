/*
 * MQTT.hpp
 *
 *  Created on: February 15, 2020
 *      Author: Alex Konshin <akonshin@gmail.com>
 */

#ifndef MQTT_HPP_
#define MQTT_HPP_

#include <string.h>
#include <mosquittopp.h>
#include <errno.h>

#include "Logger.hpp"
#include "../common/Config.hpp"

class Config;

class MqttPublisher: public mosqpp::mosquittopp {

private:
  const char *host;
  const char *id;
  int port;
  int keepalive;
  int options;
  volatile bool connected = false;
  //PH added
  const char *username;
  const char *password;

public:
  MqttPublisher(const char* id, const char* host, int port, const char* username, const char* password, int options = 0, int keepalive = 60) : mosquittopp(id) {
    mosqpp::lib_init();      // Initialization of mosquitto library
    this->keepalive = keepalive;
    this->options = options;
    this->id = id;
    this->port = port;
    this->host = host;
    if (username == NULL || *username == '\0') {
      this->username = NULL;
      this->password = NULL;
    } else {
      this->username = username;
      this->password = password;
    }
    instance = this;
  }

  ~MqttPublisher() {
    if (connected) stop(true);
    mosqpp::lib_cleanup();   // Mosquitto library cleanup
  }

  static MqttPublisher* instance;

  static bool create(Config& cfg);

  static void destroy() {
    if (instance != NULL) {
      MqttPublisher* publisher = instance;
      instance = NULL;
      if (publisher->is_connected()) {
        Log->log("Stopping MQTT publisher...");
        publisher->stop(true);
        Log->log("MQTT publisher has been stopped.");
      }
      Log->log("Destroying MQTT publisher...");
      delete publisher;
    }
  }

  bool start() {
    Log->log("Connecting to MQTT broker %s:%d...", host, port);
    if (username != NULL) username_pw_set(username, password);
    int rc = connect(host, port, keepalive);
    switch (rc) {
    case MOSQ_ERR_SUCCESS:
      connected = true; // actually it should be already set by on_connect()
      break;
    case MOSQ_ERR_INVAL:
      Log->error("ERROR MOSQ_ERR_INVAL on connecting to MQTT broker: %s", mosqpp::strerror(rc));
      return false;
    case MOSQ_ERR_ERRNO:
      char buffer[256];
      const char* error_message;
      error_message = strerror_r(errno, buffer, 256);
      Log->error("ERROR %d on connecting to MQTT broker: %s", rc, error_message);
      // PH: Wait some seconds for the MQTT-Broker to start
      sleep(5);
      // PH: Variante 1: Abort, systemctl starts the program again 
      return false;
      // PH: Variante2: No abort, program connects to MQTT broker later
      //printf("Wait for MQTT Broker\n");
      //break;
    default:
      Log->error("ERROR %d on connecting to MQTT broker: %s", rc, mosqpp::strerror(rc));
      return false;
    }

    Log->log("Starting MQTT processing thread.");
    loop_start();  // Start the thread that processes connection/publish/subscribe requests
    //Log->log("Connecting to MQTT broker %s:%d...", host, port);
    //connect_async(host, port, keepalive);  // non blocking connection to MQTT broker
    return true;
  }

  void stop(bool force) {
    if (connected) disconnect();
    loop_stop();             // Stop the thread
  }

  bool is_connected() {
    return connected;
  }

  bool publish_message(const char* topic, const char* message) {
    int error_code =
      publish(            // Publish the message.
        NULL,             // (output) Message Id (int *) this allow to latter get status of each message
        topic,            // topic of the message
        strlen(message),  // length of the payload (message)
        message,          // payload (the message)
        1,                // QOS (quality of service:
                          //  0: The broker/client will deliver the message once, with no confirmation.
                          //  1: The broker/client will deliver the message at least once, with confirmation required.
                          //  2: The broker/client will deliver the message exactly once by using a four step handshake.
        false             // retain (boolean) - indicates if message is retained on broker or not.
    );                    // Returns:
                          //   MOSQ_ERR_SUCCESS  on success.
                          //   MOSQ_ERR_INVAL  if the input parameters were invalid.
                          //   MOSQ_ERR_NOMEM  if an out of memory condition occurred.
                          //   MOSQ_ERR_NO_CONN  if the client is not connected to a broker.
                          //   MOSQ_ERR_PROTOCOL if there is a protocol error communicating with the broker.
                          //   MOSQ_ERR_PAYLOAD_SIZE if payload length is too large.
                          //   MOSQ_ERR_MALFORMED_UTF8 if the topic is not valid UTF-8
                          //   MOSQ_ERR_QOS_NOT_SUPPORTED  if the QoS is greater than that supported by the broker.
                          //   MOSQ_ERR_OVERSIZE_PACKET  if the resulting packet would be larger than supported by the broker.
    if (error_code == MOSQ_ERR_SUCCESS) return true;
    Log->error("ERROR %d on publishing MQTT message: %s", error_code, mosqpp::strerror(error_code));
    return false;
  }

private:
  void on_connect(int rc) {
    if (rc != MOSQ_ERR_SUCCESS ) {
      Log->error("ERROR %d on connecting to MQTT broker: %s", rc, mosqpp::strerror(rc));
    } else {
      //PH: Better reporting
      //if ( (options&(VERBOSITY_INFO|VERBOSITY_DEBUG))!=0 ) Log->log("Successfully connected to MQTT broker %s:%d.", host, port);
      Log->info("Successfully connected to MQTT broker %s:%d.", host, port);
      connected = true;
    }
  }

  void on_disconnect(int rc) {
    connected = false;
    if (rc != MOSQ_ERR_SUCCESS ) {
      Log->error("Connection to MQTT broker %s:%d has been lost (error code %d).", host, port, rc);
    } else if ( (options&(VERBOSITY_INFO|VERBOSITY_DEBUG))!=0 ) {
      Log->log("Connection to MQTT broker %s:%d has been closed.", host, port);
    }
  }

  void on_publish(int mid) {
    if ( (options&VERBOSITY_DEBUG)!=0 ) {
      Log->error("Message has been sent to MQTT broker %s:%d.", host, port);
    }
  }

};


//-------------------------------------------------------------
// MQTT rule definition

class MqttRule : public AbstractRuleWithSchedule {
public:
  const char* mqttTopic;

  MqttRule(SensorDef* sensor_def, Metric metric, const char* mqttTopic) : AbstractRuleWithSchedule(sensor_def, metric) {
    this->mqttTopic = mqttTopic;
  }

  const char* getTypeName() {
    return "MQTT rule";
  }

  void execute(const char* message, class Config& cfg);

};

#endif /* MQTT_HPP_ */
