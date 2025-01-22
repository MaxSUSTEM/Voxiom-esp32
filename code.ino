#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <pgmspace.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

// WiFi настройки
const char* ssid = "name";           // Имя вашей сети Wi-Fi
const char* password = "12345678";   // Пароль от вашей сети Wi-Fi
const char* apiUrl = "https://api.tricko.pro/voxiom/player/Generator?log=gena"; //  /player/NICK?log=NICK


void connectToWiFi()
{
  Serial.print("Подключение к ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi подключен.");
  Serial.print("IP-адрес: ");
  Serial.println(WiFi.localIP());
}

String getApiData() {
  HTTPClient http;
  http.begin(apiUrl);
  int httpResponseCode = http.GET();
  String payload = "{}";  
  if (httpResponseCode > 0)
  {
    payload = http.getString();
  } else 
  {
    Serial.print("Ошибка при получении данных: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return payload;
  }
void parseJson(String jsonString)
  {
  StaticJsonDocument<3072> doc; 
  DeserializationError error = deserializeJson(doc, jsonString);
    if (error) {
    Serial.print("Ошибка разбора JSON: ");
    Serial.println(error.c_str());
    return;
   }
     bool success = doc["success"];
    JsonObject data = doc["data"];
    const char* data_nickname = data["nickname"];
    bool data_has_nickname = data["has_nickname"];
    long long data_creation_time = data["creation_time"];
    int data_level = data["level"];
    int data_xp = data["xp"];
    int data_gems = data["gems"];
    long data_score = data["score"];
      lcd.setCursor(0, 0);
     lcd.print(data_nickname);
      lcd.setCursor(10, 0);
     lcd.print("LVL:");
      lcd.setCursor(14,0);
     lcd.print(data_level);
      lcd.setCursor(0,1);
     lcd.print("S:");
      lcd.setCursor(2,1);
     lcd.print(data_score);
      lcd.setCursor(10,1);
     lcd.print("G:");
      lcd.setCursor(12,1);
     lcd.print(data_gems);
    Serial.println(data_nickname);
    JsonObject data_br = data["br"];
    int data_br_total_games_played = data_br["total_games_played"];
    int data_br_total_games_won = data_br["total_games_won"];
    int data_br_total_kills = data_br["total_kills"];
    int data_br_total_deaths = data_br["total_deaths"];
    long data_br_total_survival_time = data_br["total_survival_time"];
    long data_br_total_score = data_br["total_score"];
    for (JsonObject data_br_matche : data_br["matches"].as<JsonArray>()) {
    const char* data_br_matche_match_id = data_br_matche["match_id"];
    const char* data_br_matche_time = data_br_matche["time"];
    const char* data_br_matche_map_name = data_br_matche["map_name"];
    int data_br_matche_num_players = data_br_matche["num_players"];
    int data_br_matche_duration = data_br_matche["duration"];
    int data_br_matche_rank = data_br_matche["rank"];
    int data_br_matche_kills = data_br_matche["kills"];
    int data_br_matche_deaths = data_br_matche["deaths"];
    int data_br_matche_survival_time = data_br_matche["survival_time"];
    int data_br_matche_score = data_br_matche["score"];
    Serial.println(data_br_matche_match_id);
    Serial.println(data_br_matche_score);
   }
    JsonObject data_ctg = data["ctg"];
    int data_ctg_total_games_played = data_ctg["total_games_played"];
    int data_ctg_total_games_won = data_ctg["total_games_won"];
    int data_ctg_total_kills = data_ctg["total_kills"];
    int data_ctg_total_deaths = data_ctg["total_deaths"];
    int data_ctg_total_captures = data_ctg["total_captures"];
    long data_ctg_total_score = data_ctg["total_score"];
    for (JsonObject data_ctg_matche : data_ctg["matches"].as<JsonArray>()) {
    const char* data_ctg_matche_match_id = data_ctg_matche["match_id"];
    const char* data_ctg_matche_time = data_ctg_matche["time"];
    const char* data_ctg_matche_map_name = data_ctg_matche["map_name"];
    int data_ctg_matche_duration = data_ctg_matche["duration"];
    int data_ctg_matche_team1_score = data_ctg_matche["team1_score"];
    int data_ctg_matche_team2_score = data_ctg_matche["team2_score"];
    bool data_ctg_matche_victory = data_ctg_matche["victory"];
    int data_ctg_matche_kills = data_ctg_matche["kills"];
    int data_ctg_matche_deaths = data_ctg_matche["deaths"];
    int data_ctg_matche_captures = data_ctg_matche["captures"];
    int data_ctg_matche_score = data_ctg_matche["score"];
    Serial.println(data_ctg_matche_match_id);
    Serial.println(data_ctg_matche_score);
   }
    bool data_rewarded_ad_eligible = data["rewarded_ad_eligible"];
    int data_friend_status = data["friend_status"];
    JsonObject data_clan = data["clan"];
    int data_clan_clan_id = data_clan["clan_id"];
    const char* data_clan_name = data_clan["name"];
    const char* data_clan_tag = data_clan["tag"];
    Serial.println(data_clan_name);
    Serial.println(data_clan_tag);
   }
void setup()
   {
  Serial.begin(115200);  
  connectToWiFi();       
  lcd.init();                      
  lcd.init();
  lcd.backlight();
  }
void loop()
  {
  if (WiFi.status() == WL_CONNECTED) {
    String jsonData = getApiData(); 
    Serial.println("Полученные данные:");
    Serial.println(jsonData);        

    parseJson(jsonData);           
    delay(5000); //  время обновлния
    lcd.clear();
   } 
    else
   {
    Serial.println("Соединение с Wi-Fi потеряно. Попытка переподключения...");
    connectToWiFi();
   }
   }
