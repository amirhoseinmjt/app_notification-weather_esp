using System.Security.Principal;
using Telegram.Bot;
using TL;
using System;
using System.Threading.Tasks;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;

namespace csharpMQTT
{
    internal class Program
    {
        /*    
            token : 2114708151:POYygmHVbARaWc5e4Iwb7ky1QmAxCvfAejxP5EDM
            user id : 2114708151
            @esp_massages_bot
         */
        private static void Main(string[] args)
        {


            //Non_interactive();
            bale();
            Console.WriteLine("done");


        }
        static void MQTT_Main(string msg)
        {
            string broker = "w9a8ccf9.ala.us-east-1.emqxsl.com";
            int port = 1883;
            string topic = "esp32/project";
            string clientId = Guid.NewGuid().ToString();
            string username = "amir";
            string password = "1234";
            MqttClient client = ConnectMQTT(broker, port, clientId, username, password);
            Publish(client, topic, msg);
        }

        static MqttClient ConnectMQTT(string broker, int port, string clientId, string username, string password)
        {
            MqttClient client = new MqttClient(broker, port, true, MqttSslProtocols.TLSv1_1, null, null);
            client.Connect(clientId, username, password);
            if (client.IsConnected)
            {
                Console.WriteLine("Connected to MQTT Broker");
            }
            else
            {
                Console.WriteLine("Failed to connect");
            }
            return client;
        }

        static void Publish(MqttClient client, string topic, string msg)
        {
            client.Publish(topic, System.Text.Encoding.UTF8.GetBytes(msg));
            Console.WriteLine("Send `{0}` to topic `{1}`", msg, topic);
            //System.Threading.Thread.Sleep(1 * 1000);
        }

        static async void test()
        {
            await Man();
        }
        public static async void bale()
        {
            TelegramBotClient bot = new TelegramBotClient(token: "2114708151:POYygmHVbARaWc5e4Iwb7ky1QmAxCvfAejxP5EDM", baseUrl: "https://tapi.bale.ai/");
            string me = bot.GetMeAsync().Result.ToString();
            Console.WriteLine("user :" + me);
            int n = bot.GetUpdatesAsync().Result.Length;
            Console.WriteLine("number of new massages :" + n.ToString());
            string str = "Bale Bot :\n" + n.ToString() + " new massages";
            MQTT_Main(str);
        }

        static async Task Non_interactive()
        {
            // Replace these values with your own API ID, Hash, and phone number
            Console.WriteLine("v1");
            string apiId = "23650257";
            string apiHash = "b19354d974471c906235540c6abf253a";
            WTelegram.Client client = new WTelegram.Client(int.Parse(apiId), apiHash);
            Console.WriteLine("v2");
            //string rs = await client.Login("+989912792990");
            if (client.User == null)
                Console.WriteLine("yes");
            //Console.WriteLine(client.User.GetType().ToString());
            //Console.WriteLine(client.Login("+989912792990").Result);
            //await client.Login("+989912792990");
            await DoLogin("+989912792990");
            //Console.WriteLine("v3");

            async Task DoLogin(string loginInfo) // (add this method to your code)
            {
                Console.WriteLine("loginInfo :" + loginInfo);
                Console.WriteLine("v3");
                while (1 == 1)
                {
                    if (client.User == null)
                    {
                        Console.WriteLine("yes");
                    }
                    else
                    {
                        Console.WriteLine("v6");
                        Console.WriteLine($"We are logged-in as {client.User} (id {client.User.id})");
                        break;
                    }
                    //Console.WriteLine("v4");
                    //Console.WriteLine(loginInfo);
                    switch (await client.Login(loginInfo)) // returns which config is needed to continue login
                    {
                        case "api_id":
                            Console.Write("api_id: ");
                            loginInfo = apiId;
                            break;
                        case "api_hash":
                            Console.Write("api_hash ");
                            loginInfo = apiHash;
                            break;
                        case "phone_number":
                            Console.Write("phone_number ");
                            loginInfo = "+989912792990";
                            break;
                        case "verification_code":
                            Console.Write("Code: ");
                            loginInfo = Console.ReadLine();
                            break;
                        case "first_name":
                            loginInfo = "Amir";
                            break;     // if sign-up is required
                        case "last_name":
                            loginInfo = "Moj";
                            break;      // if sign-up is required
                        case "name":
                            loginInfo = "Amir Moj";
                            break;    // if sign-up is required (first/last_name)
                                      //case "password": loginInfo = "secret!"; break; // if user has enabled 2FA
                        default:
                            loginInfo = null;
                            break;
                    }
                    //Console.WriteLine(loginInfo);
                }
            }

            //using var client = new WTelegram.Client();
            /*using (var client = new WTelegram.Client(Config))
            {
                client.Auth_SignIn();
                await client.ConnectAsync();

                // Log in with your phone number
                var hash = await client.SendCodeRequestAsync(phoneNumber);
                Console.Write("Enter the code: ");
                var code = Console.ReadLine();
                var user = await client.MakeAuthAsync(phoneNumber, hash, code);

                // Get dialogs and count the number of messages
                var dialogs = await client.GetUserDialogsAsync();
                int messageCount = 0;

                foreach (var dialog in dialogs.Dialogs)
                {
                    var messages = await client.GetHistoryAsync(dialog.Peer, limit: 100);
                    messageCount += messages.Messages.Count;
                }

                Console.WriteLine($"Number of messages: {messageCount}");

                await client.DisconnectAsync();
            }*/
        }


        static async Task Man()
        {
            using var client = new WTelegram.Client();
            var myself = await client.LoginUserIfNeeded();
            Console.WriteLine($"We are logged-in as {myself} (id {myself.id})");
        }



    }
}