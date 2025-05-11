#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Wi-Fi Credentials
const char* ssid = "samannay";
const char* password = "bapukantha1";

// HTTP Basic Auth Credentials
const char* http_username = "admin";
const char* http_password = "esp8266";

// Create a web server on port 80
ESP8266WebServer server(80);

// LED Configuration
const int ledPin = LED_BUILTIN; // Change to GPIO2 if using external LED
//bool ledState = false;


//PIN CONFIGURATION FOR THE CAR BUTTONS
const int leftPin = 5;
const int rightPin = 4;
const int forwardPin = 0;
const int backwardPin = 2;


// Handle the root page
void handleRoot() {
  if (!server.authenticate(http_username, http_password)) {
    return server.requestAuthentication();
  }

  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>ESP8266 LED Control</title>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
        body { font-family: Arial; text-align: center; margin:0%; }
        
              .container {
  width: 100%;
  height: 150px;
  position: relative;
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 0 10px; /* Small padding for responsiveness */
  box-sizing: border-box;
}

.circle-btn {
  width: 100px;
  height: 100px;
  border-radius: 50%;
  border: none;
  color: white;
  font-size: 16px;
}
        
        </style>
    </head>
    <body>
      <h1>ABINASH'S CAR </h1>
      <!--<p>Status: <span id="ledState">Unknown</span></p>-->
      <button class="circle-btn" style="background-color: yellow;display:table; margin-left: auto; margin-right: auto;" id="leftPin"></button>

       <div class="container">
        <button id="backwardPin" style="background-color: orange;" class="circle-btn" ></button>
        <button id="forwardPin" style="background-color: green;" class="circle-btn" ></button>
       </div>
      <button  class="circle-btn" style=" background-color: red;display:table; margin-left: auto; margin-right: auto;" id="rightPin" ></button>
      
      <script>
      
      //LEFT
        const leftBtn = document.getElementById('leftPin');
        leftBtn.addEventListener('mousedown', () => fetch('/okLeftPin'));
        leftBtn.addEventListener('mouseup', () => fetch('/off'));
        leftBtn.addEventListener('touchstart', () => fetch('/okLeftPin'));
        leftBtn.addEventListener('touchend', () => fetch('/off'));
      //LEFT

      //RIGHT
        const rightBtn = document.getElementById('rightPin');
        rightBtn.addEventListener('mousedown', () => fetch('/okRightPin'));
        rightBtn.addEventListener('mouseup', () => fetch('/off'));
        rightBtn.addEventListener('touchstart', () => fetch('/okRightPin'));
        rightBtn.addEventListener('touchend', () => fetch('/off'));
      //RIGHT

      //FORWARD
        const forwardBtn = document.getElementById('forwardPin');
        forwardBtn.addEventListener('mousedown', () =>fetch('/okForwardPin'));
        forwardBtn.addEventListener('mouseup', () =>fetch('/off'));
        forwardBtn.addEventListener('touchstart', () =>fetch('/okForwardPin'));
        forwardBtn.addEventListener('touchend', () =>fetch('/off'));
      //FORWARD  
      
      //BACKWARD
        const backwardBtn = document.getElementById('backwardPin');
        backwardBtn.addEventListener('mousedown', () => fetch('/okBackwardPin'));
        backwardBtn.addEventListener('mouseup', () => fetch('/off'));
        backwardBtn.addEventListener('touchstart', () => fetch('/okBackwardPin'));
        backwardBtn.addEventListener('touchend', () => fetch('/off'));
      //BACKWARD
      
      </script>
    </body>
    </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

// Handle turning the car LEFT
void handleLeft() {
  if (!server.authenticate(http_username, http_password)) {
    return server.requestAuthentication();
  }
  digitalWrite(leftPin, HIGH); 
  digitalWrite(rightPin,LOW);
  digitalWrite(forwardPin,LOW);
  digitalWrite(backwardPin,LOW);
  server.send(200, "text/plain", "OK");
}

// Handle turning the car RIGHT
void handleRight() {
  if (!server.authenticate(http_username, http_password)) {
    return server.requestAuthentication();
  }
  digitalWrite(rightPin, HIGH); 
  digitalWrite(leftPin,LOW);
  digitalWrite(forwardPin,LOW);
  digitalWrite(backwardPin,LOW);
  server.send(200, "text/plain", "OK");
}


//MOVING FORWARD
void handleForward() {
  if (!server.authenticate(http_username, http_password)) {
    return server.requestAuthentication();
  }
  
  digitalWrite(leftPin,HIGH);//5
  digitalWrite(rightPin, LOW); //4
  digitalWrite(forwardPin,HIGH);//0
  digitalWrite(backwardPin,LOW);//2
  server.send(200, "text/plain", "OK");
}

//MOVING BACKWARD
void handleBackward() {
  if (!server.authenticate(http_username, http_password)) {
    return server.requestAuthentication();
  }
  
  digitalWrite(leftPin,LOW); //5
  digitalWrite(rightPin, HIGH); //4
  digitalWrite(forwardPin,LOW);//0
  digitalWrite(backwardPin,HIGH);//2
  server.send(200, "text/plain", "OK");
}


// Handle turning the CAR OFF
void handleOff() {
  if (!server.authenticate(http_username, http_password)) {
    return server.requestAuthentication();
  }
  digitalWrite(leftPin,LOW);
  digitalWrite(rightPin,LOW);
  digitalWrite(forwardPin,LOW);
  digitalWrite(backwardPin,LOW);
  server.send(200, "text/plain", "OK");
}


void setup() {
  Serial.begin(115200);
  pinMode(leftPin,OUTPUT);
  pinMode(rightPin,OUTPUT);
  pinMode(forwardPin,OUTPUT);
  pinMode(backwardPin,OUTPUT);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define web routes
  server.on("/", handleRoot);
  server.on("/okLeftPin", handleLeft);
  server.on("/okRightPin", handleRight);
  server.on("/okForwardPin", handleForward);
  server.on("/okBackwardPin", handleBackward);
  server.on("/off", handleOff);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
