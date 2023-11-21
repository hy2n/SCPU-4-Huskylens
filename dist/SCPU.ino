int Line_select = 0;                                //선택된 라인
int Line_count = 0;                                 //감지된 라인
int Line_X[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //X값변수
int Line_Y[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //Y값변수
int Line_Height[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int Line_Width[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
int X_LR = 0;        //왼쪽오른쪽구분
int X_root = 0;      //양모서리거리값
int X_io = 0;        //서보회전값
int X_MinValue = 0;  //차선허용범위 (양옆값)
int X_notTurn = 1;   //90도 회전 여부


void loop() {
  while (RedLine) {
    ScanTurn();
    if (RedLine) LineTrace(150);
  }  
}
void ScanBlock(int id) {
  if (huskylens.request(id)) {
    Line_count = huskylens.count(id);
    int i = 1;
    while (i < (huskylens.count(id) + 1)) {
      HUSKYLENSResult result = huskylens.get(id, (i - 1));
      Line_X[i] = result.xCenter;  //변수에 값 저장
      Line_Y[i] = result.yCenter;
      Line_Width[i] = result.width;
      Line_Height[i] = result.height;
      i++;
    }
  } else {
    Line_count = 0;
  }
}

void ScanBlockCAM2(int id) {
  if (huskylens2.request(id)) {
    Line_count = huskylens2.count(id);
    int i = 1;
    while (i < (huskylens2.count(id) + 1)) {
      HUSKYLENSResult result2 = huskylens2.get(id, (i - 1));
      Line_X[i] = result2.xCenter;  //변수에 값 저장
      Line_Y[i] = result2.yCenter;
      Line_Width[i] = result2.width;
      Line_Height[i] = result2.height;
      i++;
    }
  } else {
    Line_count = 0;
  }
}


int ScanTurn() {
  ScanBlock(2);
  if (Line_count > 0) {
    if (Line_count == 1) Line_select = 1;                        // 1개 이상의 객체가 발견돼면
    else if (Line_count == 2) Line_select = DefineObject(1, 2);  //2개 발견하면 일반 Define해서 선택
    else Line_select = AdvancedDefine();                         //3개이상 발견하면 고급 Define해서 선택
    if ((Line_Y[Line_select] > 160)) {
      RedLine = 0;
    } else {
      RedLine = 1;
    }
  } else {
    RedLine = 1;
  }
}

int ScanCAM2(int objID) {
  ScanBlockCAM2(objID);  //2번 ID 물체 검색
  if (Line_count > 0) {
    if (Line_count == 1) Line_select = 1;                                     // 1개 이상의 객체가 발견돼면
    else if (Line_count == 2) Line_select = DefineObject(1, 2);               //2개 발견하면 일반 Define해서 선택
    else Line_select = AdvancedDefine();                                      //3개이상 발견하면 고급 Define해서 선택
    if ((Line_Width[Line_select] < 40) && (Line_Height[Line_select] < 40)) {  //너무 작으면 패스
      decOBJ = 0;                                                             //미발견 (대상 물체 너무 작음)
    } else {
      decOBJ = Line_select;
    }
  } else {
    decOBJ = 0;
  }
}

int ScanCAM1(int objID) {
  ScanBlock(objID);  //물체 검색
  if (Line_count > 0) {
    if (Line_count == 1) Line_select = 1;                                     // 1개 이상의 객체가 발견돼면
    else if (Line_count == 2) Line_select = DefineObject(1, 2);               //2개 발견하면 일반 Define해서 선택
    else Line_select = AdvancedDefine();                                      //3개이상 발견하면 고급 Define해서 선택
    if ((Line_Width[Line_select] < 50) && (Line_Height[Line_select] < 50)) {  //너무 작으면 패스
      decOBJ = 0;                                                             //미발견 (대상 물체 너무 작음)
    } else {
      decOBJ = Line_select;
    }
  } else {
    decOBJ = 0;
  }
}

void LineTrace(int speed) {
  ScanBlock(1);
  if (Line_count > 1) {                                     // 1개 이상의 객체가 발견돼면
    if (Line_count == 2) Line_select = DefineObject(1, 2);  //2개 발견하면 일반 Define해서 선택
    else Line_select = AdvancedDefine();                    //3개이상 발견하면 고급 Define해서 선택
  } else {
    Line_select = 1;  //1개 발견시 그거로 선택
  }
  X_range_define(Line_select);

  X_io = X_root * 0.7;  //계산값
  X_io = (int)X_io;
  if (Line_count == 0) Forward(speed);
  else if (X_root > 20) {
    if (X_LR == 1) {
      SRight(130, X_io);
    } else {
      SLeft(130, X_io);
    }
  }
}

void LineTrace_slow(int speed) {
  ScanBlock(1);
  if (Line_count > 1) {                                     // 1개 이상의 객체가 발견돼면
    if (Line_count == 2) Line_select = DefineObject(1, 2);  //2개 발견하면 일반 Define해서 선택
    else Line_select = AdvancedDefine();                    //3개이상 발견하면 고급 Define해서 선택
  } else {
    Line_select = 1;  //1개 발견시 그거로 선택
  }
  X_range_define(Line_select);

  X_io = X_root * 0.7;  //계산값
  X_io = (int)X_io;
  if (Line_count == 0) Forward(speed);
  else if (X_root > 10) {
    if (X_LR == 1) {
      SRight(130, X_io);
    } else {
      SLeft(130, X_io);
    }
  }
}

int DefineObject(int object1, int object2) {
  if (((Line_Width[object1] < 100) || (Line_Width[object2] < 100)) && ((Line_Height[object1] < 60) || (Line_Height[object2] < 60))) {
  } else if (Line_Y[object1] > Line_Y[object2]) return object1;
  else if (Line_Y[object1] < Line_Y[object2]) return object2;
}

int X_range_define(int objID) {
  if (Line_X[objID] > 160) {
    X_root = 320 - Line_X[objID];
    X_LR = 2;
  }  // 오른쪽 축 계산
  else {
    X_root = Line_X[objID];
    X_LR = 1;
  }  //왼쪽 축 계산
}

int AdvancedDefine() {  //가장 큰 오브젝트로 선택한다
  int k = 1;
  int max = 1;
  int maxID = 1;
  while (k < (Line_count + 1)) {
    if ((Line_Width[k] < 100) && (Line_Height[k] < 60)) {  //너무 작으면 패스

    } else if (Line_Width[k] > max) {
      max = Line_Width[k];
      maxID = k;
    }
    k++;
  }
  return maxID;
}

void WaitObjectCAM1(int objID) {  // objID이름의 오브젝트 대기
  ObjectnotDected = 1;            //초기화
  while (ObjectnotDected)
    if (huskylens.request(objID))
      if (huskylens.count() < 1) {  //오브젝트가 없다면
        delay(500);                 //감지시 정확성을 위해 5초후 재검증
        if (huskylens.request(objID))
          if (huskylens.count() < 1) ObjectnotDected = 0;
      }
}
