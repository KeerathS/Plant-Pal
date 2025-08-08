#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// your sensor on GPIO4 (pad next to RST on the C3-mini)
const int soilPin = 4;

// calibrate these by reading Serial.print(raw)    
const int dryRaw = 300;   
const int wetRaw = 2000;

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);  // SDA=8, SCL=9
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 init failed!");
    while (1) delay(10);
  }
}

void loop() {
  // 1) Read & map
  int raw = analogRead(soilPin);
  int moisture = map(raw, dryRaw, wetRaw, 0, 100);
  moisture = constrain(moisture, 0, 100);
  Serial.printf("raw=%d → %d%%\n", raw, moisture);

  // 2) Draw fresh frame
  display.clearDisplay();

  // — Header in small font —
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Soil Moisture:");

  // — Big numeric value —
  display.setTextSize(4);
  display.setCursor(19, 20);    // make sure you leave enough vertical room
  display.printf("%d%%", moisture);

  // 3) Push to screen
  display.display();

  delay(2000);
}
