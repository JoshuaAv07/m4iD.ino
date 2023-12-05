#include <WiFi.h>

const char* ssid = "STEREN";
const char* password = "";

WiFiServer server(80);

String header;

String output26State = "off";
String output27State = "off";

const int output26 = 26;
const int output27 = 27;


unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  Serial.begin(9600);
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);


  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available(); 
  if (client) {                            
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");                                             
    String currentLine = "";                                                   
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  
      currentTime = millis();
      if (client.available()) {  
        char c = client.read();  
        Serial.write(c);         
        header += c;
        if (c == '\n') {  
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            

            if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              output26State = "on";
              digitalWrite(output26, HIGH);

            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
            }

            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}p{font-weight:bold}");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #0D0D0D;}</style></head>");

            client.println("<body><h1>m4iD weaponry control</h1>");

            client.println("<p>GPIO 26 - State " + output26State + "</p>");
            if (output26State == "off") {
              digitalWrite(output26, LOW);
              digitalWrite(output27, HIGH);
              client.println("<p><button class=\"button\" id='fetchButton'>Activate Weapon</button></p>");
            } else {
              digitalWrite(output26, HIGH);
              digitalWrite(output27, LOW);
              client.println("<p><button class=\"button\" id='fetchButton'>Activate Weapon</button></p>");
            }

            client.println("<div id='stats'>");
            client.println("<div id='weapon-state-indicator' class='weapon-state-indicator'></div>");
            client.println("<div id='text-weapon-state-indicator' class='text-weapon-state-indicator'>Weapon deactivated</div>");
            client.println("<div class='text-weapon-state-indicator'>Active state updated with id: </div>");
            client.println("<div id='database-state-indicator' class='text-weapon-state-indicator'></div>");
            client.println("</div>");
            client.println("<script>console.log('JsWorks');const URL = 'http://127.0.0.1:3000/arduino/create';const options = { method:'POST', headers: { 'Content-Type': 'application/json', }, body: JSON.stringify({ arduino: 'joshua', team: 'm4iD' }) };const fetchButton = document.getElementById('fetchButton');const textWeaponStateIndicator = document.getElementById('text-weapon-state-indicator');const weaponStateIndicator = document.getElementById('weapon-state-indicator');const databaseStateIndicator = document.getElementById('database-state-indicator');let weaponActiveState = false;fetchButton.addEventListener('click', async function () { console.log('Button Works'); weaponActiveState = !weaponActiveState; textWeaponStateIndicator.innerHTML = weaponActiveState ? 'Weapon Activated' : 'Weapon deactivated'; weaponStateIndicator.style.backgroundColor = weaponActiveState ? '#00ff00' : '#ff0000'; try{ const response = await fetch(URL, options); if(response.ok){ const result = await response.json(); databaseStateIndicator.innerHTML = result.insertedId; console.log(result.insertedId); } console.log(response.ok); }catch(err){ console.log(err); } });</script>");
            client.println("</body></html>");

            client.println();
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r')
          currentLine += c;      
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}