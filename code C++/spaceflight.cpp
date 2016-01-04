// By: Kurt Ahonen
// help from: Dr. Pankratz and Hanna Raczek

////////////////////////////////////////////////////////////////////////////////////////
/////////// add hiscores ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

#include "DarkGDK.h"
#include "sncIOstream.h"
using namespace std;

const int BOTTOM_EDGE    = 417;
const int RIGHT_EDGE     = 641;
//int ballcolorID          = 2;
const int BALL_SPRITE_ID = 2;
const int POINTER_IMAGE  = 109;
const int POINTER_SPRITE = 108;
const int OFFSET_X       = 33;
const int OFFSET_Y       = 28;
 int MAX_GOOD       = 5;
 int MAX_BAD        = 5;
const int good_image_00  = 10;
const int good_image_01  = 11;
const int good_image_02  = 12;
const int good_image_03  = 13;
const int bad_image_00   = 20;
const int bad_image_01   = 21;

const int RED = dbRGB(255,0,0);
const int WHITE = dbRGB(255,255,255);
const unsigned int BLACK = dbRGB(0,0,0);

int cnt;
int spriteID;
int cX, cY;
int temppic;
int good_id;
int lives;
int playagain = 0;
int stage2 = 0;

void screenDots(int &, bool &);
void blackBox(int &);
void displayEnd(int , int , int&, bool&);
int sncMouseFullClick(int &, int &);
int playlevel(int , int, int, bool&);
void moveBackground (int , int &);
bool sncOnSprite(int , int , int);
void updateMouse();
void initMouse();
void StartSprite(int, int );
void setup();
void DarkGDK()
{
	
START:

	lives = 5;
	temppic = 0;
	cnt = 0;

	int barY = 1;
	int x, y;
	int mouseX = 0;
	int	mouseY = 0;
	int id;
	int cnt = 0;
	int backid = 0;
	float backdropX = 0.5;
	float planetSpeed = 0.5;
	int dotcount = 0;
	bool dots = false;
	int dotx, doty;
	int i, j;
	bool blackbaronce = true;
	bool playaudio = true;

	x = RIGHT_EDGE;
	y = dbRND(BOTTOM_EDGE);
	dotx = dbRND(479);
	doty = dbRND(479);

	setup();

	//title screen
	playlevel( mouseX, mouseY, playagain, playaudio);

	if (playaudio == true)
		dbPlayMusic(1);

	dbSyncOn();
	dbSyncRate(60);

	// setting the levels
	if (temppic == 1){
		MAX_BAD = 5;
		}
	if (temppic == 2){
		MAX_BAD = 10;
		}
	if (temppic == 3){
		MAX_BAD = 25;
		}

	// putting the sprites out for the first time
	for (id = 10; id < 10+MAX_GOOD; id++){
		StartSprite( id, playagain);}
	for (id = 20; id < 20+MAX_BAD; id++){
		StartSprite( id, playagain);}

	while ( !dbEscapeKey() )
	{
		dotcount++;
		backdropX = backdropX - planetSpeed;
		dbPasteImage( 70 + backid, backdropX, 0);

		// displays level on screen
		if (temppic == 1){
			sncCenterText(50, 15, "level 1");
			}
		if (temppic == 2){
			sncCenterText(50, 15, "level 2");
			}
		if (temppic == 3){
			sncCenterText(50, 15, "level 3");
			}

		//  displays score
		sncCenterText(320, 20, "Score:");
		sncCenterText(320, 32, cnt);

		// displays lives
		sncCenterText(530, 20, "Lives Left:");
		sncCenterText(530, 32, lives);
		
		mouseX = dbMouseX();
		mouseY = dbMouseY();

		updateMouse();

		// decending black lines
		if (cnt >= 7 && cnt <= 9 && lives > 0){
			blackBox(barY);
			
			while (blackbaronce == true)
			{
				dbShowMouse();
				dbSprite(124, 157, 240, 124);
				dbSync();
				dbWait(3000);
				dbDeleteSprite(124);
				blackbaronce = false;
			}
		}

		//dots
		if (cnt >= 16 && cnt <= 18)
		{
			screenDots(dotcount, dots);
		}
		if (cnt == 19)
			dbDeleteSprite(126);
			

		// loop for the good sprites
		for (id = 10; id < 10+MAX_GOOD; id++)
		{
			int x = dbSpriteX(id);
			int y = dbSpriteY(id);
			x = x - 5;
			int IMAGE = dbSpriteImage(id);
			dbSprite(id, x, y, IMAGE);
		
			// checking if mouse is on a sprite
			if (sncOnSprite( id, mouseX, mouseY) == true){
				StartSprite( id, playagain);
				cnt++;
				if (playaudio == true)
					dbPlaySound(1);}

			//checking if a sprite went off the screen
			if (x < -126){
				StartSprite( id, playagain);
				}
		}

		// Loop for the bad sprites
		for (id = 20; id < 20+MAX_BAD; id++)
		{

			int x = dbSpriteX(id);
			int y = dbSpriteY(id);
			x = x - 5;
			int IMAGE = dbSpriteImage(id);
			dbSprite(id, x, y, IMAGE);
		
			// checking if mouse is on a bad sprite
			if (sncOnSprite( id, mouseX, mouseY) == true){
				dbShowMouse();
				StartSprite( id, playagain);
				lives--;
				if (playaudio == true)
					dbPlaySound(2);

				while (LoopGDK())
				{
					dbShowMouse();
					dbSprite(122, 122, 240, 122);
					dbSync();
					dbWaitKey();
					dbDeleteSprite(122);
					break;
				}
				
			}
			
			//checking if a sprite went off the screen
			if (x < -126){
				StartSprite( id, playagain);
				}
		}
			
			// gives the background a neverending loop
			if (backdropX <= -2075 && backid == 0){
				backdropX = 640;
				backid = 1;}
			if (backdropX <= -2075 && backid == 1){
				backdropX = 640;
				backid = -1;}

		// checks for end game and displays screen
		if ( playagain == 2)
		{
			if (lives <= 0 || cnt >= 25)
			{
				displayEnd( cnt, lives, playagain, playaudio);

				if (playagain == 1)
					goto START;
				if (playagain == 2){
					goto START;}
				if (playagain == 0)
					break;
			}
		}

		if (playagain != 2)
		{
			if (lives <= 0 || cnt >= 15)
			{
				displayEnd( cnt, lives, playagain, playaudio);

				if (playagain == 1)
					goto START;
				if (playagain == 2){
					goto START;}
				if (playagain == 0)
					break;
			}
		}

		dbSync();
	}//end game loop
}//DarkGDK



//*************************************************************
// The onSprite function takes a sprite number and a set of   *
// XY coordinates as arguments.. The function returns true    *
// if the coordinates are located on the sprite's bounding    *
// rectangle, or false otherwise.                             *
//*************************************************************
bool sncOnSprite(int spriteID, int x, int y) {

	// Get the coordinates of the sprite's upper-left corner.
	//The insertion point may have been moved by (OffsetX,OffsetY).
    int upperX = dbSpriteX(spriteID) - dbSpriteOffsetX(spriteID);
    int upperY = dbSpriteY(spriteID) - dbSpriteOffsetY(spriteID);

    // Get the coordinates of the sprite's lower-right corner.
    int lowerX = upperX + dbSpriteWidth(spriteID);
    int lowerY = upperY + dbSpriteHeight(spriteID);

    // (x,y) inside or outside sprite?
	if (x >= upperX && x <= lowerX && y >= upperY && y <= lowerY)
	{
		return true;
	}
	else
		return false;
	//endif
} //sncOnSprite


//*******************************************************
// If the user is pressing a mouse button *
// it gets the mouse pointer's coordinates and then     *
// waits for the user to release the button. It returns *
// 0 = false if no button is pressed *
// 1 = true if left is pressed
// 2 = true if right is pressed
// 3 = true if left && right are pressed
//*******************************************************
int sncMouseFullClick(int &cX, int &cY)
{
    int click;
	// Get the mouse pointer coordinates.
    click = dbMouseClick(); // 0,1,2 or 3
	if ( click) //not zero
	{
		cX = dbMouseX();
		cY = dbMouseY();
		while (dbMouseClick()) 
		{
		   // busy/wait loop
		}
	}
	return click;
} //sncMouseFullClick

void setup()
{
	initMouse();

	dbRandomize( dbTimer());

	dbSetImageColorKey(0,255,0);  // green
	
	dbLoadImage("alien01.bmp",20);
	dbLoadImage("alien02.bmp",21);
	dbLoadImage("goodstar1.bmp",10);
	dbLoadImage("goodstar2.bmp",11);
	dbLoadImage("goodstar3.bmp",12);
	dbLoadImage("stage2star.bmp", 13);
	dbLoadImage("easy.bmp",6);
	dbLoadImage("medium.bmp",7);
	dbLoadImage("hard.bmp",8);
	//dbLoadImage("BlueBall2.bmp",9);
	//dbLoadImage("PurpleBall2.bmp",10);
	//dbLoadImage("WhiteBall2.bmp",11);
	dbLoadImage("background.bmp",70);
	dbLoadImage("background2.bmp",71);
	dbLoadImage("titlebackground.bmp",100);
	dbLoadImage("wooden_background.bmp",120);
	dbLoadImage("unstoppable.bmp",121);
	dbLoadImage("badhit.bmp", 122);
	dbLoadImage("newgoal.bmp", 123);
	dbLoadImage("losingconnection.bmp", 124);
	dbLoadImage("entername.bmp", 125);
	dbLoadImage("losingconnection2.bmp", 126);
	dbLoadImage("audioon.bmp", 127);
	dbLoadImage("audiooff.bmp", 128);
	dbLoadImage("yes.bmp", 30);
	dbLoadImage("no.bmp", 31);

	if (!dbImageExist(10) || !dbImageExist(11) || !dbImageExist(12) || !dbImageExist(13) || !dbImageExist(20)
		|| !dbImageExist(21) || !dbImageExist(70) || !dbImageExist(71) || !dbImageExist(100)){
		sncCenterText(100, 50,"Cannot find a picture ..Press a key"); dbWaitKey(); return;
	}

	dbLoadSound("punch.wav", 1);
	dbLoadSound("woosh.wav", 2);
	dbLoadSound("exp03.wav", 3);
	dbLoadSound("unstoppable.wav", 4);
	dbLoadMusic("adventure_music01.wav", 1);

	dbSprite(127, 580, 10, 127);
	dbSprite(128, 580, 10, 128);
	dbDeleteSprite(127);dbDeleteSprite(128);
	
	return;
}

void StartSprite(int id, int playagain)
{
	// checks if sprite was good
	if (id <= 10 + MAX_GOOD && id >= 10)
	{

		int IMAGE = dbRND(2);
		if (playagain == 2)
			IMAGE = 3;

		int x = dbRND(RIGHT_EDGE) + 640;
		int y = dbRND(BOTTOM_EDGE);

		if (IMAGE == 0)
			IMAGE = good_image_00;
		else if (IMAGE == 1)
			IMAGE = good_image_01;
		else if (IMAGE == 2)
			IMAGE = good_image_02;
		else if (IMAGE == 3)
			IMAGE = good_image_03;

		dbSprite(id, x, y, IMAGE);
	}
	
	// checks if sprite was bad
	else if (id <= 20 + MAX_BAD && id >= 20)
	{
		int IMAGE = dbRND(1);

		int x = dbRND(RIGHT_EDGE) + 640;
		int y = dbRND(BOTTOM_EDGE);

		if (IMAGE == 0)
			IMAGE = bad_image_00;
		else
			IMAGE = bad_image_01;

		dbSprite(id, x, y, IMAGE);
	}

	dbSetSpritePriority(id, 0);
	dbWait(25);
}

void initMouse()
{
	dbHideMouse();
	dbSetImageColorKey(0,255,0);

	int centerX = dbScreenWidth() / 2;
	int centerY = dbScreenHeight() / 2;
	dbPositionMouse(centerX, centerY);

	dbLoadImage("ship.bmp", POINTER_IMAGE);
	dbSprite(POINTER_SPRITE, centerX, centerY, POINTER_IMAGE);
	dbDeleteSprite(POINTER_SPRITE);
	

	dbSetSpritePriority(POINTER_SPRITE, 1);
}

void updateMouse()
{	
	dbHideMouse();
	// returns current mouse coordinates
	int mouseX = dbMouseX();
	int mouseY = dbMouseY();

	dbSetImageColorKey(0,255,0);
	//displays ship image to mouse
	dbOffsetSprite(POINTER_SPRITE, OFFSET_X, OFFSET_Y);
	dbSprite(POINTER_SPRITE, mouseX, mouseY, POINTER_IMAGE);
	
}

int playlevel(int mouseX, int mouseY, int playagain, bool& playaudio)
{
	dbShowMouse();
	bool clicked = false;
	string name;

	while ( clicked == false)
	{
		dbInk(BLACK, 0);
		dbPasteImage(125, 0, 0, 125);
		cin >> name;
		clicked = true;
		dbInk(WHITE, 0);
	}

	clicked = false;
	while ( clicked == false)
	{
		// loads sprites for each difficulty
		dbSprite(1, 49 , 367, 6);
		dbSprite(2, 161 , 367, 7);
		dbSprite(3, 273 , 367, 8);
		dbSprite(127, 580, 10, 127);

		if ( playagain == 2)
			dbSprite(123, 192, 240, 123);

		dbPasteImage( 100 , 0 , 0);
		
		mouseX = dbMouseX();
		mouseY = dbMouseY();

		// allows user to scroll over an image to see it's difficulty
		if (sncOnSprite( 1, mouseX, mouseY)){
			dbInk(RED,0);
			sncCenterText(82, 433, "SANDBOX");}

		if (sncOnSprite( 2, mouseX, mouseY)){
			dbInk(RED,0);
			sncCenterText(193, 433, "MEDIUM");}

		if (sncOnSprite( 3, mouseX, mouseY)){
			dbInk(RED,0);
			sncCenterText(306, 433, "EXPERT");}

		dbInk(WHITE,0);

		// allows user to click on and choose difficulty. Also plays sound and deletes current sprites
		if (sncOnSprite( 1, mouseX, mouseY) == true && sncMouseFullClick( cX, cY)){
			temppic = 1;
			clicked = true;
			dbDeleteSprite(1);
			dbDeleteSprite(2);
			dbDeleteSprite(3);
			dbDeleteSprite(123);
			dbDeleteSprite(127);dbDeleteSprite(128);
			if (playaudio == true)
				dbPlaySound(3);}
		if (sncOnSprite( 2, mouseX, mouseY) == true && sncMouseFullClick( cX, cY)){
			temppic = 2;
			clicked = true;
			dbDeleteSprite(1);
			dbDeleteSprite(2);
			dbDeleteSprite(3);
			dbDeleteSprite(123);
			dbDeleteSprite(127);dbDeleteSprite(128);
			if (playaudio == true)
				dbPlaySound(3);}
		if (sncOnSprite( 3, mouseX, mouseY) == true && sncMouseFullClick( cX, cY)){
			temppic = 3;
			clicked = true;
			dbDeleteSprite(1);
			dbDeleteSprite(2);
			dbDeleteSprite(3);
			dbDeleteSprite(123);
			dbDeleteSprite(127);dbDeleteSprite(128);
			if (playaudio == true)
				dbPlaySound(3);}
		if (sncOnSprite( 127, mouseX, mouseY) == true && sncMouseFullClick( cX, cY)){
			playaudio = false;
			dbDeleteSprite(127);
			dbSprite(128, 580, 70, 128);}
		if (sncOnSprite( 128, mouseX, mouseY) == true && sncMouseFullClick( cX, cY)){
			playaudio = true;
			dbDeleteSprite(128);
			dbSprite(127, 580, 10, 127);}

		dbSync();
	}

	dbCLS();
	dbWait(25);
	return temppic;
}

void displayEnd(int cnt, int lives, int& playagain, bool& playaudio)
{	
	dbWait(200);

	bool tempsound = true;

	dbShowMouse();
	int mouseX, mouseY;
	bool clicked = false;

	//delete all sprites
	dbDeleteSprite(108);

	int id;
	for (id = 10; id < 10+MAX_GOOD; id++)
		dbDeleteSprite(id);
	for (id = 20; id < 20+MAX_BAD; id++)
		dbDeleteSprite(id);

	dbCLS();

	if (lives == 5)
		if (playaudio == true)
			dbPlaySound(4);

	while (clicked == false && !dbEscapeKey())
	{
		dbCLS();

		dbPasteImage( 120 , 0 , 0);
		if (lives == 5)
			dbPasteImage(121, 0, 0);


		dbInk(WHITE, 0);

		if ( lives <= 0)
			sncCenterText(320, 185, "You LOST");
		else if ( cnt >= 10 && cnt <= 20)
			sncCenterText(320, 185, "You WON");
		if (playagain == 2)
			sncCenterText(320, 150, "!!! YOU CONQUERED THE STARS !!!");

		sncCenterText(320, 200, "Stars retrieved:");
		sncCenterText(320, 215, cnt);
		sncCenterText(320, 230, "Play again?");

		dbSprite(30, 150 , 300, 30);
		dbSprite(31, 390 , 300, 31);
		
		mouseX = dbMouseX();
		mouseY = dbMouseY();

		dbInk(BLACK, 0);
		if (sncOnSprite( 30, mouseX, mouseY)){
			sncCenterText(200 , 415, "Just 1 more!");}
		if (sncOnSprite( 31, mouseX, mouseY)){
			sncCenterText(440, 415, "Tally Ho!");}
		dbInk(WHITE, 0);

		if (sncOnSprite( 30, mouseX, mouseY) == true && sncMouseFullClick( cX, cY)){
			clicked = true;
			dbDeleteSprite(30);
			dbDeleteSprite(31);
			if (playaudio == true)
				dbPlaySound(3);
			playagain = 1;}
		if (sncOnSprite( 31, mouseX, mouseY) == true && sncMouseFullClick( cX, cY)){
			clicked = true;
			dbDeleteSprite(30);
			dbDeleteSprite(31);
			if (playaudio == true)
				dbPlaySound(3);
			playagain = 0;}

		if ( dbSpaceKey() )
		{
			int stage2;
			cin >> stage2;
			if ( stage2 == 1234 )
				playagain = 2;
			clicked = true;
			dbDeleteSprite(30);
			dbDeleteSprite(31);
			if (playaudio == true)
				dbPlaySound(3);
		}

		dbSync();
	}
}

void blackBox(int &barY)
{
	dbInk(BLACK, 0);
	dbBox( 0, 0, 640, barY);
	barY = barY + 2;
	dbInk(WHITE, 0);

	// checks if black lines has reached the bottom
	if (barY >= 480)
	{
		lives = 0;
	}

	return;
}

void screenDots(int &dotcount, bool &dots)
{
	int dotx, doty;
	int i, j;

	dotx = dbRND(479);
	doty = dbRND(479);

	dbSprite(126, 172, 70, 126);
	
	for (i = dotcount; i >= 0; i--)
	{
		i--;
		dotx = dbRND(640);
		doty = dbRND(480);
		dbInk(BLACK, 0);

		for (j = 15; j >= 0; j--){
			dbCircle(dotx,doty, j);}
		dbInk(WHITE, 0);
	}
		
	if(dots == true){
		dotcount = 0;
		dots = false;}
}