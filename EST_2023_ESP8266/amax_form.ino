#include <EEPROM.h>


// Define the EEPROM addresses for each variable
#define VAR1_ADDRESS 54
#define VAR2_ADDRESS 58
#define VAR3_ADDRESS 62
#define VAR4_ADDRESS 66

void xsetup() {
EEPROM.begin(EEPROM_SIZE);
YONOVAwebServer.on("/form", HTTP_GET, [](AsyncWebServerRequest* request) {
    String htmlResponse = "<!DOCTYPE html>"
                          "<html>"
                          "<head>"
                          "<title>Variable Form</title>"
                          "<style>"
                          "body {"
                          "  font-family: Arial, sans-serif;"
                          "  margin: 0;"
                          "  padding: 20px;"
                          "}"
                          "h1 {"
                          "  text-align: center;"
                          "}"
                          "form {"
                          "  max-width: 600px;"
                          "  margin: 0 auto;"
                          "  padding: 20px;"
                          "  background-color: #f2f2f2;"
                          "  border-radius: 5px;"
                          "}"
                          "label {"
                          "  display: block;"
                          "  margin-bottom: 10px;"
                          "  font-weight: bold;"
                          "}"
                          "input[type=\"text\"] {"
                          "  width: 100%;"
                          "  padding: 10px;"
                          "  margin-bottom: 20px;"
                          "  border: 1px solid #ccc;"
                          "  border-radius: 4px;"
                          "}"
                          "input[type=\"submit\"] {"
                          "  width: 100%;"
                          "  padding: 10px;"
                          "  background-color: #4CAF50;"
                          "  color: white;"
                          "  border: none;"
                          "  border-radius: 4px;"
                          "  cursor: pointer;"
                          "}"
                          "h2 {"
                          "  margin-top: 20px;"
                          "}"
                          "p {"
                          "  margin-bottom: 5px;"
                          "}"
                          "@media (max-width: 600px) {"
                          "  form {"
                          "    padding: 10px;"
                          "  }"
                          "}"
                          "</style>"
                          "</head>"
                          "<body>"
                          "<h1>Variable Form</h1>"
                          "<form action=\"/submit\" method=\"GET\">"
                          "<label for=\"var1\">Variable 1:</label>"
                          "<input type=\"text\" id=\"var1\" name=\"var1\" placeholder=\"Stored Value 1\" required>"
                          "<label for=\"var2\">Variable 2:</label>"
                          "<input type=\"text\" id=\"var2\" name=\"var2\" placeholder=\"Stored Value 2\" required>"
                          "<label for=\"var3\">Variable 3:</label>"
                          "<input type=\"text\" id=\"var3\" name=\"var3\" placeholder=\"Stored Value 3\" required>"
                          "<label for=\"var4\">Variable 4:</label>"
                          "<input type=\"text\" id=\"var4\" name=\"var4\" placeholder=\"Stored Value 4\" required>"
                          "<input type=\"submit\" value=\"Submit\">"
                          "</form>"
                          "<script>"
                          "fetch('/getValues')"
                          ".then(response => response.json())"
                          ".then(data => {"
                          "document.getElementById('var1').placeholder = data.var1;"
                          "document.getElementById('var2').placeholder = data.var2;"
                          "document.getElementById('var3').placeholder = data.var3;"
                          "document.getElementById('var4').placeholder = data.var4;"
                          "document.getElementById('storedVar1').textContent = data.var1;"
                          "document.getElementById('storedVar2').textContent = data.var2;"
                          "document.getElementById('storedVar3').textContent = data.var3;"
                          "document.getElementById('storedVar4').textContent = data.var4;"
                          "});"
                          "</script>"
                          "</body>"
                          "</html>";

    request->send(200, "text/html", htmlResponse);
});




  YONOVAwebServer.on("/submit", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (request->hasParam("var1") && request->hasParam("var2") && request->hasParam("var3") && request->hasParam("var4")) {
      int var1 = request->getParam("var1")->value().toInt();
      int var2 = request->getParam("var2")->value().toInt();
      int var3 = request->getParam("var3")->value().toInt();
      int var4 = request->getParam("var4")->value().toInt();

      EEPROM.put(VAR1_ADDRESS, var1);
      EEPROM.put(VAR2_ADDRESS, var2);
      EEPROM.put(VAR3_ADDRESS, var3);
      EEPROM.put(VAR4_ADDRESS, var4);
      EEPROM.commit();

      request->send(200, "text/plain", "Values submitted successfully");
    } else {
      request->send(400, "text/plain", "Missing required parameters");
    }
  });

  YONOVAwebServer.on("/getValues", HTTP_GET, [](AsyncWebServerRequest* request) {
    int var1, var2, var3, var4;
    EEPROM.get(VAR1_ADDRESS, var1);
    EEPROM.get(VAR2_ADDRESS, var2);
    EEPROM.get(VAR3_ADDRESS, var3);
    EEPROM.get(VAR4_ADDRESS, var4);

    String jsonResponse = "{\"var1\":" + String(var1) + ",\"var2\":" + String(var2) + ",\"var3\":" + String(var3) + ",\"var4\":" + String(var4) + "}";
    request->send(200, "application/json", jsonResponse);
  });
}