#include <TFT_eSPI.h>
#include <SPI.h>
#include <little_fs/little_fs.h>
#include <dinosaur_model/dinosaur_model.h>//恐龙模块

#define UP       8//UP
#define DOWN     13//DOWN
#define LEFT     5//LEFT
#define RIGHT    9//RIGHT
#define CENTER   4//CENTER

TFT_eSPI    tft = TFT_eSPI();        
TFT_eSprite sprite = TFT_eSprite(&tft);

unsigned long previousMillis_draw_dinasour = 0; 
unsigned long previousMillis_road_move = 0;  
unsigned long previousMillis_cloud_move = 0;  
uint8_t obstacles_index=0;//产生障碍物的序号
uint16_t score=0;//分数
uint16_t max_score=0;//最高分数暂存变量

Object dinosaur(30,32,10,49,dinosaur_1_img);//恐龙
Object cactus_1(10,19,159,60,cactus_1_img);//单个仙人掌
Object cactus_2(17,32,159,47,cactus_2_img);//单个仙人掌
Object cactuses_1(50,33,60,46,cactuses_1_img);//仙人掌从
Object cactuses_2(40,27,60,52,cactuses_2_img);//仙人掌从
Object cactuses_3(27,27,60,52,cactuses_3_img);//仙人掌从
Object road_1(160,10,0,72,road_1_img),road_2(160,10,159,72,road_1_img);//道路
Object cloud_1(30,8,60,10,cloud_img),cloud_2(30,8,40,20,cloud_img),cloud_3(30,8,140,10,cloud_img);//云彩
Object clouds[3]={cloud_1,cloud_2,cloud_3};//云彩数组
Object obstacles[5]={cactus_1,cactus_2,cactuses_1,cactuses_2,cactuses_3};//障碍物数组


void init();

void setup() {
  init();
}

void loop() {
  game_start();
}

void init()
{
  tft.init();

  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  sprite.setSwapBytes(true);
  
  sprite.fillSprite(0x0000);
  sprite.setTextSize(1);
  sprite.setTextColor(TFT_PINK);
  sprite.fillScreen(TFT_BLACK);
  pinMode(UP,INPUT_PULLUP);
  pinMode(DOWN,INPUT_PULLUP);
  pinMode(LEFT,INPUT_PULLUP);
  pinMode(RIGHT,INPUT_PULLUP);
  pinMode(CENTER,INPUT_PULLUP);
  randomSeed(38);
  LittleFS.begin(true);
}
void pushImage(Object obj)//包装绘制方法
{
  sprite.pushImage(obj.index_x,obj.index_y,obj.width,obj.height,obj.data);
}
bool draw()//绘制整个游戏画面
{

  sprite.createSprite(160,80);
  draw_road();
  draw_obstacle();
  draw_cloud();
  draw_dinosaur();
  bool game_status=collision_detection(dinosaur,obstacles[obstacles_index]);//碰撞检测
  sprite.drawString(String(score),0,0);
  sprite.pushSprite(0, 0);
  sprite.deleteSprite();
  return game_status;//返回游戏状态
}
void draw_road()//绘制路面
{
  pushImage(road_1);
  pushImage(road_2);
  road_move();//路面移动
}
void draw_dinosaur()//绘制小恐龙
{
  static bool flag=true;//恐龙变换
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis_draw_dinasour >= 100) {
    previousMillis_draw_dinasour = currentMillis;
    flag=!flag;
  }
  if(flag)
  {
    dinosaur.data=dinosaur_1_img;
    pushImage(dinosaur);
  }
  else
  {
    dinosaur.data=dinosaur_2_img;
    pushImage(dinosaur);
  } 
}
void draw_obstacle()//绘制障碍物
{
    pushImage(obstacles[obstacles_index]);
}
void draw_cloud()//绘制云彩
{
    for(int i=0;i<3;i++)
    {
      pushImage(clouds[i]);
    }
     cloud_move();
}
bool dinosaur_move()//小恐龙跳跃移动
{
      for(int i=0;i<80;i++)
      {
        --dinosaur.index_y;
        if(!draw())
        {
          return true;
        }
      }
     
     for(int i=0;i<80;i++)
      {
        ++dinosaur.index_y;
        if(!draw())
        {
          return true;
        }
      }
      return false;
}
void road_move()//路面移动
{
   unsigned long currentMillis = millis();
    if (currentMillis - previousMillis_road_move >= 3) {
      previousMillis_road_move = currentMillis;
      --road_1.index_x;
      --road_2.index_x;
      if(road_1.index_x==-road_1.width) road_1.index_x=159;
      if(road_2.index_x==-road_2.width)road_2.index_x=159;
      obstacle_move();//障碍物和路面同步移动
    }
}
void obstacle_move()//障碍物移动
{
  --obstacles[obstacles_index].index_x;
  if(obstacles[obstacles_index].index_x==-obstacles[obstacles_index].width)
  {
    
    randomSeed(random(0,200));
    obstacles_index=random(0,5);
    obstacles[obstacles_index].index_x=159;//当前障碍物位置复位
    ++score;//加分
  }
}
void cloud_move()//云彩移动
{
  unsigned long currentMillis = millis();
    if (currentMillis - previousMillis_cloud_move >= 30) {
      previousMillis_cloud_move = currentMillis;
      for(int i=0;i<3;i++)
      {
        clouds[i].index_x--;
        if(clouds[i].index_x==-clouds[i].width)//云彩整个超出屏幕
        {
          clouds[i].index_x=159;//返回起点
          clouds[i].index_y=random(0,20);//随机高度
        }
      }
    }
}
void game_init()//游戏初始化
{
  score=0;//分数清0
  randomSeed(random(0,200));
  obstacles_index=random(0,3);//产生一个新的障碍物并放置在起点
  obstacles[obstacles_index].index_x=159;
  dinosaur.index_y=49;//小恐龙归位
}
void game_start()//开始小恐龙游戏
{
  static bool game_start=false;//游戏开始标志
  bool game_status;//游戏状态标志
  bool max_score_once=true;
  if(digitalRead(UP)==0)game_start=true;
  if(game_start)
  {
    if(digitalRead(CENTER)==0)//小恐龙跳跃
    {
      game_status=dinosaur_move();
    }
    if(!draw()||game_status)//游戏结束
    { 
      while(true)
      {
        if(max_score_once)//执行一次分数结算
        {
          set_max_score();
          max_score_once=false;
        }
        game_over();
        if(digitalRead(UP)==0)
        {
          delay(200);
          if(digitalRead(UP)==0)
          {
            game_init();//重新开始游戏(初始化游戏)
            break;
          }
        }
      } 
    }
  }
  else
  {
    sprite.createSprite(160,80);
    sprite.fillScreen(TFT_BLACK);
    sprite.setTextSize(2);
    sprite.drawString("Play Game!",20,30);
    sprite.setTextSize(1);
    sprite.pushSprite(0, 0);
    sprite.deleteSprite();
  }
  
}
void game_over()//小恐龙游戏结束
{
  sprite.createSprite(160,80);
  sprite.fillScreen(TFT_BLACK);
  sprite.setTextSize(2);
  sprite.drawString("Game Over!",20,30);
  
  if(score==max_score)//当前分数为新的最高分
  {
    String temp="New Max:"+String(max_score);
    sprite.setTextColor(TFT_GOLD);
    sprite.drawString(temp,18,50);
  }
  else
  {
    String temp="Max Score:"+String(max_score);
    sprite.setTextColor(TFT_ORANGE);
    sprite.drawString(temp,15,50);
  }
   sprite.setTextColor(TFT_PINK);
  sprite.setTextSize(1);
  sprite.pushSprite(0, 0);
  sprite.deleteSprite();
}
bool collision_detection(Object dinosaur,Object obstacle)//小恐龙与障碍物碰撞监测
{
  bool flag=false;
  for(int i=dinosaur.index_x;i<dinosaur.index_x+dinosaur.width;i++)
  { 
    for(int j=obstacle.index_x;j<obstacle.index_x+obstacle.width;j++)
    {
      if (i==j)
      {
        for(int k=dinosaur.index_y;k<dinosaur.index_y+dinosaur.height;k++)
          {
            for(int l=obstacle.index_y;l<obstacle.index_y+obstacle.height;l++)
            {
              if (k==l)
              {
                if((((dinosaur.index_y+25)<k)&&(i<7))||(((dinosaur.index_y+21)<k))&&(i>20))continue;//屏蔽一些影响方块
                return false;
              }
            }
          }
      }
    }
  }
  
  return true;
}
void set_max_score()//最高分获取与设置
{
    String max_score_str=readFile(LittleFS,"/score.txt");
    if(String("failed").equals(max_score_str))//找不到文件
    {
        writeFile(LittleFS, "/score.txt", String(score).c_str());//创建一个新的文件
    }
    else 
    {
        max_score=max_score_str.toInt();
      if(score>max_score)//更新最高分
      {
        writeFile(LittleFS, "/score.txt", String(score).c_str());
        max_score=score;//更新全局最高分变量
      }
    }
 
}
