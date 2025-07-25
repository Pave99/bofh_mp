extern ACTOR actor[];
extern BOMB bomb[];
extern CLOSET closet[];
extern LIFT lift[];
extern STAIRS stairs[];

extern int numstairs;
extern int numclosets;
extern int numlifts;
extern int numbombs;

extern int bombs;
extern ACTOR *speaker;
extern int weapon;
extern int gametime;
extern int showinstr;
extern int showinstrtime;
extern int paused;
extern int score;
extern HISCORE_ENTRY hiscore[];
extern int terrorists;
extern int leaders;
extern int computers;
extern int gameover;
extern int xpos, ypos;
extern char mutesound;
extern char mouseinitted;
extern int mousemovex;
extern int mousemovey;
extern int prevmovex;
extern int prevmovey;
extern int avgmovex;
extern int avgmovey;
extern unsigned mouseb;
extern unsigned prevmouseb;
extern int difficulty;
extern int gamespeed;
extern unsigned char *firebuf;
extern char textbuf[];
extern int ammo[WEAPNUM];
extern const int maxammo[WEAPNUM];
extern const int attackdelaytbl[];
extern int actorradius[];
extern int fistpr[];
extern int pistolpr[];
extern int shotgunpr[];
extern int uzipr[];
extern int flamepr[];
extern int crossbowpr[];
extern int comphealth[];
extern int enemyhealth[];
extern int enemyscore[];
extern int deadenemytype[];
extern int deadenemyitem[];
extern int bnd_hittime;
extern enum bnd_sound bnd_sound;
extern char *gamemsg;
extern int gamemsgtime;
extern int lifecheat;
extern int timecheat;
extern int ammocheat;
extern int speedcheat;
extern int instrcheat;
extern int enemycheat;
extern int grenadecheat;
extern int xshift;
extern int yshift;
extern int trapmsg;
extern int victory;
extern int throwstrength;
extern int fastforward;
extern int liftsound;
extern int scannerdelay;
extern int actors;
extern int kills;
extern int victorybits;
extern int firstdamagedblock;
extern int srminx, srminy, srmaxx, srmaxy;
extern char briefingtext[];
extern char victorytext[];
extern int beginservers;
extern int beginworkstations;
