import telegram
import asyncio
import requests
from requests.exceptions import RequestException
import time
from paho.mqtt import client as mqtt_client
import random

def mqtt_main(massage):
    # mqtt info
    mqtt_username = "amir"
    mqtt_password = "1234"
    topic = "esp32/project"
    mqtt_broker = "w9a8ccf9.ala.us-east-1.emqxsl.com"
    mqtt_port = 8883
    auth = {'username': mqtt_username, 'password': mqtt_password}
    client_id = f'python-mqtt-{random.randint(0, 1000)}'
    #print("----------------------------------------")
    #publish_msg2(topic=topic,massage_NU=massage,mqtt_broker=mqtt_broker,mqtt_port=mqtt_port,auth=auth)
    #print("----------------------------------------")
    ca_cert = \
        "-----BEGIN CERTIFICATE-----\n" \
        "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
        "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
        "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
        "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
        "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
        "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
        "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
        "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
        "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
        "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
        "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
        "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
        "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
        "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
        "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
        "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
        "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
        "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
        "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
        "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n" \
        "-----END CERTIFICATE-----\n"
    #print(" --->mqtt_main")
    client = connect_mqtt(client_id=client_id,broker=mqtt_broker,mqtt_port=mqtt_port,password=mqtt_password,username=mqtt_username)
    client.loop_start()
    publish_massage(client=client,topic=topic,massage_NU=massage)
    client.loop_stop()
    #print("mqtt_main <---")

def connect_mqtt(client_id,broker,mqtt_port,password,username):
    #print("---> connect_mqtt")

    def on_connect(client, userdata, flags, rc):
        #print(str(rc))
        if rc == 0:
            print("Connected to MQTT Broker!")
        else:
            print("Failed to connect, return code %d\n", rc)
    # Set Connecting Client ID
    client = mqtt_client.Client(client_id)
    client.username_pw_set(username, password)
    client.tls_set(ca_certs='./ca_cert.crt')
    client.on_connect = on_connect
    client.connect(broker, mqtt_port)
    return client

def publish_massage(client,topic,massage_NU):
    #publish.single(topic, massage_NU, hostname=mqtt_broker, port=mqtt_port, auth=auth)
    msg_count = 1
    while True:
        time.sleep(1)
        #msg = f"messages: {msg_count}"
        result = client.publish(topic, massage_NU)
        # result: [0, 1]
        status = result[0]
        if status == 0:
            print(f"Send `{massage_NU}` to topic `{topic}`")
            break
        else:
            print(f"Failed to send message to topic {topic}")
        msg_count += 1
        if msg_count > 5:
            break

def test_telegram_connection():
    #telegram bot info
    url = "https://api.telegram.org/bot6780863786:AAFiY2-CDOCrumd7oV6W3_B1x6chhqFZFWw/getMe"
    max_retries = 3
    retries = 0

    while retries < max_retries:
        try:
            response = requests.get(url)
            response.raise_for_status()  # Raises an HTTPError for bad responses
            print("Connection to Telegram API successful.")
            break  # Break out of the loop if successful
        except RequestException as e:
            print(f"Error connecting to Telegram API: {e}")
            retries += 1
            time.sleep(1)  # Wait for 1 second before retrying

bot = telegram.Bot(token='6780863786:AAFiY2-CDOCrumd7oV6W3_B1x6chhqFZFWw')
bale_bot = telegram.Bot(token="2114708151:POYygmHVbARaWc5e4Iwb7ky1QmAxCvfAejxP5EDM", base_url="https://tapi.bale.ai/")

async def bale_get_new_message_count():
    last_update_id_file = 'bale_last_update_id.txt'
    try:
        with open(last_update_id_file, 'r') as file:
            last_update_id = int(file.read().strip())
    except FileNotFoundError:
        last_update_id = 0

    # Retrieve updates starting from the last known update ID
    updates = await bale_bot.get_updates(offset=last_update_id + 1)
    # print(updates)
    # Update the last known update ID
    if updates:
        last_update_id = updates[-1].update_id
        with open(last_update_id_file, 'w') as file:
            file.write(str(last_update_id))

    # Count the number of new messages
    new_message_count = len(updates)
    print(new_message_count)
    return new_message_count

async def get_new_message_count():
    # Get the last update ID from a file or database
    last_update_id_file = 'last_update_id.txt'
    try:
        with open(last_update_id_file, 'r') as file:
            last_update_id = int(file.read().strip())
    except FileNotFoundError:
        last_update_id = 0

    # Retrieve updates starting from the last known update ID
    updates = await bot.get_updates(offset=last_update_id + 1)
    # print(updates)
    # Update the last known update ID
    if updates:
        last_update_id = updates[-1].update_id
        with open(last_update_id_file, 'w') as file:
            file.write(str(last_update_id))

    # Count the number of new messages
    new_message_count = len(updates)
    return new_message_count

async def main():
    # Get the number of new messages
    test_telegram_connection()
    while True:
        number_messages = await get_new_message_count()
        bale_number_masages = await bale_get_new_message_count()
        str1 = "* Bale BOT:\n" + str(bale_number_masages) + " new massages"
        # print("Number of new messages:", number_messages)
        str2 = "* Telegram BOT:\n" + str(number_messages) + " new massages"
        res = str1 + "\n----------\n" + str2
        print(str1)
        print(str2)
        mqtt_main(res)
        time.sleep(90)
        print("********************************************")
    #publish_massage(strr)
    #print("Number of new messages:", number_messages)


if __name__ == '__main__':
    # Explicitly create and set an event loop
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)

    # Run the event loop
    loop.run_until_complete(main())