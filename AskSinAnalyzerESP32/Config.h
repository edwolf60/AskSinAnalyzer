#define JSONCONFIG_IP                     "ip"
#define JSONCONFIG_NETMASK                "netmask"
#define JSONCONFIG_GW                     "gw"
#define JSONCONFIG_CCUIP                  "ccuip"
#define JSONCONFIG_SVANALYZEINPUT         "svanalyzeinput"
#define JSONCONFIG_SVANALYZEOUTPUT        "svanalyzeoutput"

bool loadSystemConfig() {
  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists(CONFIG_FILENAME)) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open(CONFIG_FILENAME, "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, buf.get());
        if (error) {
          Serial.println("load Config JSON DeserializationError");
          return false;
        } else {
          JsonObject json = doc.as<JsonObject>();
          Serial.println("Content of JSON Configfile:");
          serializeJson(doc, Serial);
          Serial.println("\nJSON OK");

          ((json[JSONCONFIG_IP]).as<String>()).toCharArray(NetConfig.ip, IPSIZE);
          ((json[JSONCONFIG_NETMASK]).as<String>()).toCharArray(NetConfig.netmask, IPSIZE);
          ((json[JSONCONFIG_GW]).as<String>()).toCharArray(NetConfig.gw, IPSIZE);
          ((json[JSONCONFIG_CCUIP]).as<String>()).toCharArray(HomeMaticConfig.ccuIP, IPSIZE);
          ((json[JSONCONFIG_SVANALYZEINPUT]).as<String>()).toCharArray(HomeMaticConfig.SVAnalyzeInput, VARIABLESIZE);
          ((json[JSONCONFIG_SVANALYZEOUTPUT]).as<String>()).toCharArray(HomeMaticConfig.SVAnalyzeOutput, VARIABLESIZE);
        }
      }
    } else return false;
  } else return false;
  return true;
}

bool saveSystemConfig() {
  Serial.println("saving config");
  StaticJsonDocument<1024> doc;
  JsonObject json = doc.to<JsonObject>();

  json[JSONCONFIG_IP] = NetConfig.ip;
  json[JSONCONFIG_NETMASK] = NetConfig.netmask;
  json[JSONCONFIG_GW] = NetConfig.gw;
  json[JSONCONFIG_CCUIP] = HomeMaticConfig.ccuIP;
  json[JSONCONFIG_SVANALYZEINPUT] = HomeMaticConfig.SVAnalyzeInput;
  json[JSONCONFIG_SVANALYZEOUTPUT] = HomeMaticConfig.SVAnalyzeOutput;

  File configFile = SPIFFS.open(CONFIG_FILENAME, "w");
  if (!configFile) {
    Serial.println("failed to open config file for writing");
  }
  serializeJson(doc, Serial);
  Serial.println();
  serializeJson(doc, configFile);
  configFile.close();
}
