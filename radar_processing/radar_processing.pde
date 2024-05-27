import processing.serial.*;
 
Serial myPort;
int rad = 0;
float cx, cy;
ArrayList<Ball> balls = new ArrayList<Ball>();
 
void setup()
{
  size(400, 400);
  cx = width/2;
  cy = height*2/3;
  noFill();
  
  // 사용 가능한 시리얼 포트 출력
  String[] portNames = Serial.list();
  
  println("Available serial ports:");
  for (int i = 0; i < portNames.length; i++) {
    println(portNames[i]);
  myPort = new Serial(this, portNames[0], 9600);
  myPort.bufferUntil('\n');
  }
}
 
void draw(){
  background(0); // black background
  stroke(0, 255, 36); //bright green color
  arc(cx, cy, width, height, PI, TWO_PI, CHORD);
  float r = TWO_PI-map(rad, 0, 360, 0, TWO_PI);
  line(cx, cy, cx + cos(r)* width/2, cy + sin(r)* height/2);
  updateBalls();
  displayBalls();
}
 
void updateBalls(){
  for(int i = balls.size()-1 ; i > -1 ; i--){
    balls.get(i).update();
    if(balls.get(i).isDead())
    balls.remove(i);
  }
}
 
void displayBalls(){
for(int i = 0 ; i < balls.size() - 1 ; i++){
balls.get(i).display();
}
}
 
void serialEvent(Serial myPort){
  String inString = myPort.readStringUntil('\n');
  if(inString != null){
    println(inString);
    if(inString.startsWith("Angle:")){
      String[] strings = inString.replace("Angle: ","").trim().split("  Distance: ");
      if(strings.length > 1){
        int rad = Integer.parseInt(strings[0].trim());
        int distance = Integer.parseInt(strings[1].replace(" cm",""));
      if(distance != 0){
        balls.add(new Ball(cx, cy, rad, distance));
        }
      }
    }
  }
}
 
class Ball{
  int life = 50;
  float x, y;
  public Ball(float cx, float cy, int rad, int distance){
  float d = map(distance, 0, 100, 0, width/2);
  float r = TWO_PI-map(rad, 0, 360, 0, TWO_PI);
  x = cx + cos(r)*d;
  y = cy + sin(r)*d;
}
public void display(){
  ellipse(x, y, life, life);
}
public void update(){
  --life;
  if(life < 0 )
  life = 0;
  }
  public boolean isDead(){
    return life == 0;
  }
}
