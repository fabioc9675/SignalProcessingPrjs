
int pulsador = 2;
int u;

double yk = 0;
double uk_1 = 0;
double yk_1 = 0;

double c1 = 0.7788;
double c2 = 0.2212;

int i = 0;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(pulsador, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:

  yk = c1*yk_1 + c2*uk_1;

  uk_1 = u;
  yk_1 = yk;

  u = digitalRead(pulsador);

  Serial.println(yk);
  delay(250);


}
