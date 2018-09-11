#include <ESP8266WiFi.h>

const char* ssid = "viettel1234";
const char* password = "1223334444";

int ledPin = 13; // GPIO13
WiFiServer server(80);   							//Cài đặt module ESP8266 thành server.
//Note 80, that is the standard port number used for web access.
void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);  //Kết nối wifi

  while (WiFi.status() != WL_CONNECTED) {    		//Kiểm tra tình trạng kết nối
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();          							//Bắt đầu chạy server
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());						// Đọc địa chỉ ip của Server
  Serial.println("/");

}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();			// Gets a client that is connected to the server and has data available for reading.
  if (!client) {
    return; // if client =0 -> stay here or =1 exit
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){   					//Returns the number of bytes available for reading
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');    //\r : Nhảy về đầu hàng, không xuống hàng.
  Serial.println(request);
  client.flush();									//Bỏ qua các phần chưa đọc.

  // Match the request

  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {			//The index of val within the String, or -1 if not found.
    //indexOf: Locates a character or String within another String
    //val: the value to search for - char or String
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }

// Set ledPin according to the request
//digitalWrite(ledPin, value);

  // Return the response
  client.println("HTTP/1.1 200 OK");//Those look like the server headers and should not be presented by the browser. HTTP/1.1 200 ok.
  // Means that the server is responding using the HTTP protocol version 1.1. 200 is the code used when everything is ok
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
  client.println("<style>");
  client.println("body {background-color: lightblue;}");
  client.println("h1 {color: white;text-align: center;}");
  client.println("p {font-family: verdana;font-size: 20px;}");
  client.println("</style>");
  client.println("</head>");
  client.println("<body>");
  client.println("<h1>DA NANG UNIVERSITY OF SCIENCE AND TECHNOLOGY </h1>");
  client.println("<h2>DESIGNER : TRAN SANG</h2>");
  client.println("<h2>HOTLINE  : 0964.500.940</h2>");
  client.println("</body>");
  client.print("STATUS OF LED: ");

  if(value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}
