#include <IRremote.h>     //khai báo thư viện remote
#include "OneButton.h"    //khai báo thư viện button
//khai báo các chân I/O
const int RECV_PIN = 2; //chân nhận tín hiệu hồng ngoại
const int pinTuner = 6; //chân điều khiểm motor túp năng
const int pinSpeed1 = 3; //.....
const int pinSpeed2 = 4; //..... 3 chân tốc độ quạt
const int pinSpeed3 = 5; //.....
const int pinLedTimer1 = 10; //.... hai chân led biểu thị
const int pinLedTimer2 = 11; //.... chế độ hẹn giờ
//
byte nutnhan1 = A3; //
byte nutnhan2 = A2;
byte nutnhan3 = A1;
byte nutnhan4 = A0;
//int gtnutnhan1;
//int gtnutnhan2;
//int gtnutnhan3;
//int gtnutnhan4;
OneButton button1(nutnhan1, true);
OneButton button2(nutnhan2, true);
OneButton button3(nutnhan3, true);
OneButton button4(nutnhan4, true);
//const int led4 = 9;
//const int led1 = 10;
//const int led2 = 11;
//const int led3 = 12;
const int coi = 12;
//const int trangthai_led1 = LOW;
//const int trangthai_led2 = LOW;
//const int trangthai_led3 = LOW;
//const int trangthai_led4 = LOW;
int dem1 = 0;
int dem2 = 0;
int dem3 = 0;
int dem4 = 0;
//
unsigned long lastTime = millis();  //biến lastTime để giữ thời gian cuối cùng khi nhận tín hiệu remote

bool allowRunTimer = false;
bool running = false;
uint32_t countTimer = 0;
byte indexTimer = 0;
byte indexSpeed = 0;
//=================================================
void setup() {
  Serial.begin(9600); //khai báo cổng Serial để truyền dữ liệu nhận đc từ remote hồng ngoại lên máy tính 9600 là tốc độ baud
  //pinMode(led1, OUTPUT); //cấu hình các chân output đầu ra
 // pinMode(led2, OUTPUT);
  // pinMode(led3, OUTPUT);
  //pinMode(led4, OUTPUT);
  pinMode(coi, OUTPUT);
  pinMode(pinSpeed1, OUTPUT);
  pinMode(pinSpeed2, OUTPUT);
  pinMode(pinSpeed3, OUTPUT);
  pinMode(pinTuner, OUTPUT);
  pinMode(pinLedTimer1, OUTPUT);
  pinMode(pinLedTimer2, OUTPUT);
  digitalWrite(pinSpeed1, LOW); //set các chân đầu ra ở mức thấp
  digitalWrite(pinSpeed2, LOW);
  digitalWrite(pinSpeed3, LOW);
  digitalWrite(pinTuner, LOW);
  digitalWrite(pinLedTimer1, LOW);
  digitalWrite(pinLedTimer2, LOW);
 // digitalWrite(led1, LOW);
  //digitalWrite(led2, LOW);
  //digitalWrite(led3, LOW);
  //digitalWrite(led4, LOW);
  digitalWrite(coi, LOW);
  button1.attachClick(nhan_don);
  button2.attachClick(nhan_don1);
  button3.attachClick(nhan_don2);
  button4.attachClick(nhan_don3);

  //Khởi động bộ thu hồng ngoại
  IrReceiver.begin(RECV_PIN, true, 13); //khai báo gồm 3 tham số đầu vào: chân nhận tín hiệu: tham số cho phép led phản hồi nhấp nháy khi nhận đc tín hiệu từ remote hồng ngoại phát ra:
  //chân led phản hồi mặc định trên bord arduino là chân số 13
  //hàm setup chỉ khởi động 1 lần duy nhất khi khởi động nguồn
}
//=================================================
void loop() {
  //gtnutnhan1 = digitalRead(nutnhan1);
  //gtnutnhan2 = digitalRead(nutnhan2);
  //gtnutnhan3 = digitalRead(nutnhan3);
  //gtnutnhan4 = digitalRead(nutnhan4);
  //Serial.print("Nút nhấn: "); Serial.println(gtnutnhan);
  button1.tick(); //Kiểm tra trạng thái nút nhấn
  button2.tick(); //Kiểm tra trạng thái nút nhấn
  button3.tick();
  button4.tick();
  delay(10);


  //kiểm tra liên tục bộ nhận remote có nhận đc tín hiệu hay k
  if (IrReceiver.decode()) {
    uint32_t dataRemote = IrReceiver.decodedIRData.decodedRawData; // nếu nhận đc thì lấy giá trị vừa nhận đưa vào biến dataRemote để xử lý
    if (dataRemote > 0) { //dữ liệu hợp lệ phải > 0 nên dữ liệu nào lớn hơn 0 mới xử lý
      Serial.println(dataRemote); //sau khi có dữ liệu hợp lệ mình truyền dữ liệu đó lên máy tính để phục vụ mục đích tiếp theo
      if (millis() - lastTime > 250) { //do tắt mở trên cùng 1 phím nên phải có thời gian nghỉ giữa hai lần bấm  ở đây chọn 250ms = 0.25s
        switch (dataRemote) {
          case 4261481227  : { //phim so1 = ON/SPEED // so sánh dữ liệu nếu dữ liệu là phím 1 thì tương ứng vs phím mở quạt và tăng tốc độ
              running = true;
              indexSpeed++; //khi nhận dữ liệu thì tăng biến index lên 1, bình thường lúc khởi động nguồn lên thì nó bằng 0
              if (indexSpeed > 3) indexSpeed = 1; // kiểm tra biến index lớn hơn 3 hay k nếu lớn hơn 3 thì khởi tạo về lại 1 vì chỉ có 3 mức tốc độ
              switch (indexSpeed) {
                case 1: { //nếu bằng 1
                    keu();
                    digitalWrite(pinSpeed1, HIGH); //xuất đầu ra chân tốc độ 1 lên mức cao
                    digitalWrite(pinSpeed2, LOW);
                    digitalWrite(pinSpeed3, LOW);
                    //digitalWrite(led1, HIGH); //xuất đầu ra chân led1 biểu thị tốc độ 1 lên mức cao
                    //digitalWrite(led2, LOW);
                    // digitalWrite(led3, LOW);
                    dem1 = 1; //tương ứng với nút nhấn cũng đang ở mức 1 //tại một thời điểm chỉ có 1 chân đc tích cực
                  }
                  break;
                case 2: {
                    keu();
                    digitalWrite(pinSpeed1, LOW);
                    digitalWrite(pinSpeed2, HIGH);
                    digitalWrite(pinSpeed3, LOW);
                    //digitalWrite(led2, HIGH);
                    //digitalWrite(led1, LOW);
                    //digitalWrite(led3, LOW);
                    dem1 = 2;
                  }
                  break;
                case 3: {
                    keu();
                    digitalWrite(pinSpeed1, LOW);
                    digitalWrite(pinSpeed2, LOW);
                    digitalWrite(pinSpeed3, HIGH);
                    // digitalWrite(led3, HIGH);
                   // digitalWrite(led2, LOW);
                   // digitalWrite(led1, LOW);
                    dem1 = 3;
                  }
                  break;
              }
              running = true; //set biến running=true để báo các hàm hoặc chương trình con khác biết quạt đang chạy
            }
            break;
          case 4244769547: { //phim so2 = SWING nếu dữ liệu nhận đc là phím 2 tức là phím điều khiển motor túp năng
              if (running) { //nếu quạt đang chạy thì mới xử lý
                keu();
                int quay;
                quay = digitalRead(pinTuner);
                digitalWrite(pinTuner, !quay); //đảo trạng thái motor túp năng
                //trangthai_led4 = !trangthai_led4;
                //digitalWrite(led4, !digitalRead(led4)); //đảo trạng thái led biểu thị motor túp năng

              }
            }
            break;
          case 4228057867: { //phim so3 = TIMER nếu dữ liệu nhận đc là phím 3 tức là phím hẹn giờ
              if (running) { //nếu quạt đang chạy thì mới xử lý
                keu();
                indexTimer++; // tăng biến index
                if (indexTimer > 3) indexTimer = 0; //nếu lớn hơn 3 thì reset về 0
                switch (indexTimer) {
                  case 0: { //nếu hẹn giờ = 0 thì k có thời gian hẹn đồng thời led báo sẽ k hoạt động
                      countTimer = 0;
                      digitalWrite(pinLedTimer1, LOW);
                      digitalWrite(pinLedTimer2, LOW);
                      allowRunTimer = false; //biến cho phép chạy timer bằng false
                      dem3 = 0;
                    }
                    break;
                  case 1: { //nếu chế độ hẹn giờ =1 thì tương đương với hẹn giờ 15p, bắt đầu khởi tạo bộ timer
                      keu();
                      countTimer = millis() + 15000; //900000 = 15*1000 = 15 giây; //khởi tạo bằng cách lấy thời gian milli giây hiện tại + với tgian mong muốn của mk là 15p
                      digitalWrite(pinLedTimer1, HIGH); //xuất ra chân led số 1 báo hẹn 15p sáng
                      digitalWrite(pinLedTimer2, LOW); //
                      allowRunTimer = true; //set biến cho phép chạy timer
                      dem3 = 1;
                    }
                    break;
                  case 2: { //nếu chế độ hẹn giờ =2 thì tương đương với hẹn giờ 30p
                      keu();
                      countTimer = millis() + 30000;//1800000 = 30*1000 = 30 phút;
                      digitalWrite(pinLedTimer1, LOW);
                      digitalWrite(pinLedTimer2, HIGH); ////xuất ra chân led số 2 báo hẹn 30p sáng
                      allowRunTimer = true;
                      dem3 = 2;
                    }
                    break;
                  case 3: { //nếu chế độ hẹn giờ =3 thì tương đương với hẹn giờ 60p
                      keu();
                      countTimer = millis() + 60000;//3600000 = 60*1000 = 60 phút;
                      digitalWrite(pinLedTimer1, HIGH);
                      digitalWrite(pinLedTimer2, HIGH);
                      allowRunTimer = true;
                      dem3 = 3;
                    }
                    break;
                }
              }
            }
            break;
          case 4211346187: { //phim 4= OFF tắt tất cả
              keu();
              offAll();
            }
            break;
        }
      }
    }
    lastTime = millis();
    IrReceiver.resume(); // Cho phép nhận giá trị tiếp theo
  }
  if (allowRunTimer) {
    if (countTimer < millis()) {
      offAll();
    }
  }
}
//==========================================================
void offAll() {
  digitalWrite(pinSpeed1, LOW);
  digitalWrite(pinSpeed2, LOW);
  digitalWrite(pinSpeed3, LOW);
  digitalWrite(pinTuner, LOW);
  digitalWrite(pinLedTimer1, LOW);
  digitalWrite(pinLedTimer2, LOW);
 // digitalWrite(led2, LOW);
 // digitalWrite(led1, LOW);
  //digitalWrite(led3, LOW);
 // digitalWrite(led4, LOW);
  digitalWrite(coi, LOW);
  running = false;
  allowRunTimer = false;
  countTimer = 0;
  indexTimer = 0;
  indexSpeed = 0;
  dem1 = 0;
  dem2 = 0;
}
//==========================================================
void nhan_don() {
  dem1 += 1;
  running = true;
  if (dem1 > 3) dem1 = 1;
  Serial.print("Đếm 1: "); Serial.println(dem1);
  if ( dem1 == 1 ) {
    keu();
    //trangthai_led1 = !trangthai_led1;
    digitalWrite(pinSpeed1, HIGH);
    digitalWrite(pinSpeed2, LOW);
    digitalWrite(pinSpeed3, LOW);
    //digitalWrite(led1, HIGH);
    //digitalWrite(led2, LOW);
    // digitalWrite(led3, LOW);
    indexSpeed = 1;
  }
  if ( dem1 == 2) {
    keu();
    digitalWrite(pinSpeed1, LOW);
    digitalWrite(pinSpeed2, HIGH);
    digitalWrite(pinSpeed3, LOW);
    //digitalWrite(led2, HIGH);
   // digitalWrite(led1, LOW);
    // digitalWrite(led3, LOW);
    indexSpeed = 2;

  }
  if (dem1 == 3) {
    keu();
    digitalWrite(pinSpeed1, LOW);
    digitalWrite(pinSpeed2, LOW);
    digitalWrite(pinSpeed3, HIGH);
    // digitalWrite(led3, HIGH);
   // digitalWrite(led2, LOW);
   // digitalWrite(led1, LOW);
    indexSpeed = 3;

  }
}
//==========================================================
void nhan_don1() {
  if (dem1 > 0) {
    dem2 += 1;
    //if (running) {
    keu();

    digitalWrite(pinTuner, !digitalRead(pinTuner));
    //trangthai_led4 = !trangthai_led4;
   // digitalWrite(led4, !digitalRead(led4));
  }
}
//==========================================================
void nhan_don2() {
  if (dem1 > 0) {
    dem3 += 1;
    if (dem3 > 3) dem3 = 0;
    if ( dem3 == 0 ) {
      keu();
      digitalWrite(coi, LOW);
      countTimer = 0;
      digitalWrite(pinLedTimer1, LOW);
      digitalWrite(pinLedTimer2, LOW);
      allowRunTimer = false;
      indexTimer = 0;
    }
    if ( dem3 == 1 ) {
      keu();
      countTimer = millis() + 15000; //900000 = 15*60*1000 = 15 phút;
      digitalWrite(pinLedTimer1, HIGH);
      digitalWrite(pinLedTimer2, LOW);
      allowRunTimer = true;
      indexTimer = 1;
    }
    if ( dem3 == 2 ) {
      keu();
      countTimer = millis() + 30000;//1800000 = 30*60*1000 = 30 phút;
      digitalWrite(pinLedTimer1, LOW);
      digitalWrite(pinLedTimer2, HIGH);
      allowRunTimer = true;
      indexTimer = 2;
    }
    if ( dem3 == 3 ) {
      keu();
      countTimer = millis() + 60000;//3600000 = 60*60*1000 = 60 phút;
      digitalWrite(pinLedTimer1, HIGH);
      digitalWrite(pinLedTimer2, HIGH);
      allowRunTimer = true;
      indexTimer = 3;
    }
  }
}
//==========================================================
void nhan_don3() {
  dem4 += 1;
  offAll();
}
//==========================================================
void keu() {
  digitalWrite(coi, HIGH);
  delay(100);
  digitalWrite(coi, LOW);
}
