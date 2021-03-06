#include "lwip_mqtt.h"
#include "lwip/apps/mqtt.h"
#include <string.h>
#include "stm32h7xx_hal.h"
#include <main.h>


char buffer[1000];
/* The idea is to demultiplex topic and create some reference to be used in data callbacks
   Example here uses a global variable, better would be to use a member in arg
   If RAM and CPU budget allows it, the easiest implementation might be to just take a copy of
   the topic string and use it in mqtt_incoming_data_cb
*/
static int inpub_id;
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len)
{
  sprintf(buffer,"Incoming publish at topic %s with total length %u\n\r", topic, (unsigned int)tot_len);

  /* Decode topic string into a user defined reference */
  if(strcmp(topic, "print_payload") == 0) {
    inpub_id = 0;
  } else if(topic[0] == 'A') {
    /* All topics starting with 'A' might be handled at the same way */
    inpub_id = 1;
  }
  else {
    /* For all other topics */
    inpub_id = 9;
  }

}

static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags)
{
	  sprintf(buffer,"Incoming publish payload with length %d, flags %u\n\r", len, (unsigned int)flags);


  if(flags & MQTT_DATA_FLAG_LAST) {
    /* Last fragment of payload received (or whole part if payload fits receive buffer
       See MQTT_VAR_HEADER_BUFFER_LEN)  */
	  	process_mqtt_data(data, len);
    /* Call function or do action depending on reference, in this case inpub_id */

	}
}






static void mqtt_sub_request_cb(void *arg, err_t result)
{
  /* Just print the result code here for simplicity,
     normal behaviour would be to take some action if subscribe fails like
     notifying user, retry subscribe or disconnect from server */
  sprintf(buffer,"Subscribe result: %d\n\r", result);

}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
  const char * topico = arg;
  err_t err;
  if(status == MQTT_CONNECT_ACCEPTED) {
    sprintf(buffer,"mqtt_connection_cb: Successfully connected\n");

    /* Setup callback for incoming publish requests */
    mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);

    /* Subscribe to a topic named "placa" with QoS level 0, call mqtt_sub_request_cb with result */
    err = mqtt_subscribe(client, topico, 0, mqtt_sub_request_cb, arg);

    if(err != ERR_OK) {
      sprintf(buffer,"mqtt_subscribe return: %d\n", err);

    }
  } else {
    sprintf(buffer,"mqtt_connection_cb: Disconnected, reason: %d\n", status);

    /* Its more nice to be connected, so try to reconnect */
    //example_do_connect(client);
  }

}


void example_do_connect(mqtt_client_t *client, const char *topic)
{
  struct mqtt_connect_client_info_t ci;
  err_t err;

  /* Setup an empty client info structure */
  memset(&ci, 0, sizeof(ci));

  /* Minimal amount of information required is client identifier, so set it here */
  ci.client_id = "stm32h7";
  ci.client_user = "admin";
  ci.client_pass = "admin";

  /* Initiate client and connect to server, if this fails immediately an error code is returned
     otherwise mqtt_connection_cb will be called with connection result after attempting
     to establish a connection with the server.
     For now MQTT version 3.1.1 is always used */
  ip_addr_t mqttServerIP;
  IP4_ADDR(&mqttServerIP, 172, 16, 10, 236);
//  err = mqtt_client_connect(client, &mqttServerIP, MQTT_PORT, mqtt_connection_cb, 0, &ci);
  err = mqtt_client_connect(client, &mqttServerIP, MQTT_PORT, mqtt_connection_cb, topic, &ci);
while(0);
  /* For now just print the result code if something goes wrong */
  if(err != ERR_OK) {
    sprintf(buffer,"mqtt_connect return %d\n\r", err);

  }
}


/* Called when publish is complete either with sucess or failure */
static void mqtt_pub_request_cb(void *arg, err_t result)
{
  if(result != ERR_OK) {
    sprintf(buffer,"Publish result: %d\n", result);
  }
}
void example_publish(mqtt_client_t *client, void *arg)
{
  const char *pub_payload= arg;
  err_t err;
  u8_t qos = 0;
  u8_t retain = 0;
  err = mqtt_publish(client, "/presence", pub_payload, strlen(pub_payload), qos, retain, mqtt_pub_request_cb, arg);
  if(err != ERR_OK) {
    sprintf(buffer,"Publish err: %d\n\r", err);
  }
}
void publish_message(mqtt_client_t *client, void *arg, char* topic)
{
  const char *pub_payload= arg;
  err_t err;
  u8_t qos = 0;
  u8_t retain = 0;
  err = mqtt_publish(client, topic, pub_payload, strlen(pub_payload), qos, retain, mqtt_pub_request_cb, arg);
  if(err != ERR_OK) {
    sprintf(buffer,"Publish err: %d\n\r", err);
  }
}

void example_subscribe(mqtt_client_t *client, const char *topic){
	err_t err;
	u8_t qos = 0;
	err = mqtt_sub_unsub(client, topic, qos, mqtt_sub_request_cb, nanl, 1);
	if(err != ERR_OK) {
	    sprintf(buffer,"Subscribe err: %d\n\r", err);
	}
}





