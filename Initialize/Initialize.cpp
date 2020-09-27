//HEADER N LIBS:
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <sstream>
#include <fstream>
#include <tgmath.h>
using namespace std;
/*GLOBAL VARIABLES:
--> SDL_SURFACE:
    - scr: The screen!
    - col: Color.png!
    - txt: Text!
    - crt: Character!
    - crtz: Character's zoom pic!
    - psa: PortalShootingAnimation.png!
    - psaz: Portal shooting animation's zoom pic!
    - pannel: Pannels.png!
    - pannelz: Pannel's zoom pic!
    - button: Buttons.png!
    - buttonz: Button's zoom pic!
    - door: Doors.png!
    - doorz: Door's zoom pic!
    - cube: Cubes.png!
    - cubez: Cube's zoom pic!
    - wall: HideWall.png!
--> BOOL:
    - q: Quit!
    - d: Character's look direction!
    - pd[]: Check portal's direction keystate that now holding!
    - grd: Check if character is grounded or not!
    - cd: Check if the direction is changed!
    - wt: Wall tuoched!
    - wht: Wall had been tuoched!
    - jmp: Control the jumping of the character!
    - psu: Character just shoot up!
    - psr: Character just shoot right!
    - psl: Character just shoot left!
    - psd: Character just shoot down!
    - psc: Portal shooting animation's color!
    - run: Character is running!
    - ba[]: Check if a button is ative!
--> UINT32:
    - u32a: Surface pixel platform for col pointer!
    - u32b: Surface pixel platform for crt pointer!
    - u32c: Surface pixel platform for psa pointer!
    - u32d: Surface pixel platform for pannel pointer!
    - u32e: Surface pixel platform for button pointer!
    - u32f: Surface pixel platform for door pointer!
    - u32g: Surface pixel platform for cube pointer!
--> SDL_COLOR:
    - cl: Font's color!
--> TTF_FONT:
    - ttf: DFont.ttf!
--> INT:
    - t1: Starting time!
    - t2: Ending - starting time!
    - t3: Jump's starting time!
    - t4: Run starting time!
    - crtx: character's coordinates!
    - crty: character's coordinates!
    - crth: character's height!
    - crtw: character's w!
    - fps: Frame/sec!
    - jmph: Jump height!
    - fallh: Fall height!
    - map[][]: The map!
    - z: Variable for zooming function!
    - crtp[][]: Portal's coordinates!
    - pa[][]: Check if portal is ative or not and the status of the portal!
    - rine: Run inertia!
    - fine: Fall inertia!
    - f: frame counting varible!
    - crtcx: Character's image's clip's coordinates!
    - crtcy: Character's image's clip's coordinates!
    - f1: Frame counting variable for CrtAnimation func!
    - crx: Redundant of character's image!
    - cry: Redundant of character's image!
    - rh: Run holding variable!
    - pdp: Distance between portal and character when shooting!
    - pal: Portal shooting animation's alpha!
    - dir: The shooting direction!
    - zz: Used for zoom function!
    - zzz: Used for zoom function!
    - zov: The zoom variable!
    - anomap[][]: The animation and object map!
    - idbutton[][]: The id of buttons!
    - c: Number of buttons in the map!
    - c1: Number of cubes in the map!
    - cube[][]: Check the [0,1] cube's coordinates, [2] if the cube is ground 
tuoched, [3] the cube's fallh, [4] if cube tuoched wall, [5] if cube had been 
tuoched by wall!, [6] inertia up, [7] inertia right, [8] inertia down, [9] inertia left!
    - p: Item that choosed!
    - cid: ID of the cube that the charater is now picking up!
    - scd: Shooting animation cooldown!
    - map2: Used for spread func!
    - w: The width of the map!
    - h: The height of the map!
--> STRINGSTREAM:
    - ss: Turn int to str!
--> UINT8:
    - k: Key states!
--> SDL_EVENT:
    - e: Event!
*/
SDL_Surface *scr,*col,*txt,*crt,*crtz,*psa,*psaz,*pannel,*pannelz,*button,
*buttonz,*door,*doorz,*cube,*cubez,*wall;
Uint32 *u32a,*u32b,*u32c,*u32d,*u32e,*u32f,*u32g;
SDL_Color cl;
TTF_Font *ttf;
bool q,d,pd[4],grd,cd,wt,jmp,run,psu,psd,psl,psr,psc,wht,ba[50];
int t1,t2,crtx,crty,crtw,crth,fps,jmph,fallh,map[1000][1000],z,crtp[2][2],pa[2][2],
t3,rine,fine,f,crtcx,crtcy,f1,crx,cry,t4,rh,pdp,pal,dir,zz,zzz,zov,c,c1,p,cid,scd,
anomap[1000][1000],map2[1000][1000],idbutton[50][2],Cube[50][10],w,h;
stringstream ss;
Uint8 *k;
SDL_Event e;
//PROTOTYPES:
void initialize();
void programing();
SDL_Surface *image(string s,bool k);
void quit();
void screen();
void clrscr(int c);
void BlitSurface(SDL_Surface *img,int x,int y,int cx,int cy,int cw,int ch);
void BlitTxt(string s,int x,int y,Uint8 cl1,Uint8 cl2,Uint8 cl3);
void reset();
void CrtMovement(int mode,int spd);
void readfile();
void mapgenerator();
void zoom(int zoom);
void portal();
void teleport();
void PJFnME();
void CrtAnimation();
void PrtAnimation(int alpha,bool p);
void wallgenerator();
void UseButton();
void De_ativePortal();
void CubeMovement(int mode,int spd,int id);
void CubeEffect();
void pickup();
void CubeTlp();
void OpenDoor(int x,int y);
void CloseDoor(int x,int y,int p);
void spread(int y,int x);
void intro();
//MAIN FUNC:
int main(int argc,char *args[])
{
	initialize();
	reset();
	intro();
	programing();
	quit();
	return 1;
}
//MAIN LOOP:
void programing()
{
	bool k1 = 0;
	float fl = 0,fl1 = 0;
	int i;
	while(q == 0)
	{
		t1 = SDL_GetTicks();
		UseButton();
		De_ativePortal();
		++f;
		for(i = 0;i <= c;++i)
		ba[i] = 0;
		psu = 0,psd = 0,psl = 0,psr = 0;
		cd = 0;
		wt = 0;
		crx = 0;
		cry = 0;
		fl = 480;
		if(pal - fl / fps >= 0) pal = pal - fl / fps;
		else if(pal - fl / fps < 0) pal = 0;
		if(pal == 0) 
		{
			dir = 8;
		}
		k = SDL_GetKeyState(NULL);
		if(k[SDLK_a] && k[SDLK_d] == 0 && grd == 1 && d == 1) 
		{
			cd = 1;
			d = 0;
		}
		else if(k[SDLK_d] && k[SDLK_a] == 0 && grd == 1 && d == 0) 
		{
			cd = 1;
			d = 1;
		}
		teleport();
		CubeTlp();
		PJFnME();
		CubeEffect();
		if(k[SDLK_q]) k1 = 1;
		if(k[SDLK_q] == 0 && p < 2 && k1 == 1)
		{
		    ++p;
		    k1 = 0;
		}
		else if(k[SDLK_q] == 0 && p == 2 && k1 == 1)
		{
	    	p = 0;
	    	k1 = 0;
		}
		if((k[SDLK_UP] || k[SDLK_RIGHT] || k[SDLK_LEFT] || k[SDLK_DOWN]
		|| pd[0] == 1 || pd[3] == 1 || pd[2] == 1 || pd[1] == 1) 
		&& k[SDLK_w] == 0 && k[SDLK_s] == 0 && p < 2)
		portal();
		if((k[SDLK_UP] || k[SDLK_RIGHT] || k[SDLK_LEFT] || k[SDLK_DOWN])
		&& p == 2)
		pickup();
		else cid = -1;
		CrtAnimation();
		zoom(20);
		PrtAnimation(pal,psc);
		wallgenerator();
		fl = crx * z; fl = fl / 20;
		fl1 = cry * z; fl1 = fl1 / 20;
		BlitSurface(crtz,630 - fl,350 - fl1,0,0,80 * z,100 * z);
		mapgenerator();
		screen();
		//
		ss << "T2: " << t2;
		BlitTxt(ss.str(),4,4,0,255,255);
		ss.str("");
		ss << "D: " << d;
		BlitTxt(ss.str(),4,26,0,255,255);
		ss.str("");
		ss << "P: " << p;
		BlitTxt(ss.str(),4,48,0,255,255);
		ss.str("");
		ss << "F: " << f;
		BlitTxt(ss.str(),4,70,0,255,255);
		ss.str("'");
		ss << Cube[1][6] << " " << fine;
		BlitTxt(ss.str(),4,92,0,255,255);
		ss.str("");
		//
		SDL_Flip(scr);
		clrscr(4);
		while(SDL_PollEvent(&e))
        {
        	if(e.type == SDL_QUIT)
            {
                q = 1;
            }    
        }
		t2 = SDL_GetTicks() - t1;
		if(t2 < 1000 / fps) SDL_Delay(1000 / fps - t2);
	}
}
//INIT FUNC:
void initialize()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	scr = SDL_SetVideoMode(1281,721,32,SDL_SWSURFACE);
	SDL_WM_SetCaption("Initialize",NULL);
	IMG_Init(IMG_INIT_PNG);
	col = image("Demo/pngs/Colors.png",0);
	crt = image("Demo/pngs/crts/ClivePistonPic1.png",0);
	crtz = image("Demo/pngs/zoompic.png",1);
	psa = image("Demo/pngs/prt/PortalShootingAnimation.png",0);
	psaz = image("Demo/pngs/zoompic.png",1);
	pannel = image("Demo/pngs/objs/Pannels.png",0);
	pannelz = image("Demo/pngs/zoompic.png",1);
	button = image("Demo/pngs/objs/Buttons.png",0);
	buttonz = image("Demo/pngs/zoompic.png",1);
	door = image("Demo/pngs/objs/Doors.png",0);
	doorz = image("Demo/pngs/zoompic.png",1);
	cube = image("Demo/pngs/objs/Cubes.png",0);
	cubez = image("Demo/pngs/zoompic.png",1);
	wall = image("Demo/pngs/objs/HideWall.png",0);
	u32a = (Uint32*)col->pixels;
	u32b = (Uint32*)crt->pixels;
	u32c = (Uint32*)psa->pixels;
	u32d = (Uint32*)pannel->pixels;
	u32e = (Uint32*)button->pixels;
	u32f = (Uint32*)door->pixels;
	u32g = (Uint32*)cube->pixels;
	TTF_Init();
	ttf = TTF_OpenFont("Demo/font/Dfont.ttf",12);
}
//GET IMAGE FUNC:
SDL_Surface *image(string s,bool k)
{
	Uint32 clk;
	SDL_Surface *otmz;
	SDL_Surface *img;
	img = IMG_Load(s.c_str());
	otmz = SDL_DisplayFormat(img);
	if(k == 1)
	{
	    clk = SDL_MapRGB(otmz->format,0,255,51);
	    SDL_SetColorKey(otmz,SDL_SRCCOLORKEY,clk);
	}
	SDL_FreeSurface(img);
	return otmz;
}
//QUIT FUNC:
void quit()
{
	SDL_FreeSurface(col);
	SDL_FreeSurface(txt);
	SDL_FreeSurface(crt);
	SDL_FreeSurface(crtz);
	SDL_FreeSurface(psa);
	SDL_FreeSurface(psaz);
	SDL_FreeSurface(pannel);
	SDL_FreeSurface(pannelz);
	SDL_FreeSurface(button);
	SDL_FreeSurface(buttonz);
	SDL_FreeSurface(door);
	SDL_FreeSurface(doorz);
	SDL_FreeSurface(cube);
	SDL_FreeSurface(cubez);
	SDL_FreeSurface(wall);
	SDL_Quit();
}
//SCREEN SIDE FUNC:
void screen()
{
	SDL_Rect r;
	r.w = 3;
	r.h = 719;
	r.x = 0;
	r.y = 0;
	SDL_FillRect(scr,&r,u32a[0]);
	r.x = 1278;
	SDL_FillRect(scr,&r,u32a[0]);
	r.w = 1279;
	r.h = 3;
	r.x = 0;
	r.y = 0;
	SDL_FillRect(scr,&r,u32a[0]);
	r.y = 718;
	SDL_FillRect(scr,&r,u32a[0]);
}
//CLEAR SCREEN FUNC:
void clrscr(int c) 
{
	SDL_Rect r;
	r.x = 2;
	r.y = 2;
	r.w = 1276;
	r.h = 716;
	SDL_FillRect(scr,&r,u32a[c]);
}
//BLIT SURFACE FUNC:
void BlitSurface(SDL_Surface *img,int x,int y,int cx,int cy,int cw,int ch)
{
	SDL_Rect rect,clip;
	rect.x = x;
	rect.y = y;
	clip.x = cx;
	clip.y = cy;
	clip.h = ch;
	clip.w = cw;
	SDL_BlitSurface(img,&clip,scr,&rect);
}
//BLIT TXT FUNC:
void BlitTxt(string s,int x,int y,Uint8 cl1,Uint8 cl2,Uint8 cl3)
{
	SDL_Rect xy;
	SDL_Color cl = {cl1,cl2,cl3};
	txt = TTF_RenderText_Solid(ttf,s.c_str(),cl);
	xy.x = x;
	xy.y = y;
	SDL_BlitSurface(txt,NULL,scr,&xy);
}
//RESET FUNC:
void reset()
{
	readfile();
	fps = 24;
	z = 10;
	zz = 7392; zzz = 7392;
	q = 0;
	crtx = 100;
	crty = 50;
	crtw = z * 2;
	crth = z * 5;
	d = 1;
	jmp = 0;
	jmph = 0;
	fallh = 0;
	pd[1] = 0; pd[2] = 0; pd[3] = 0; pd[0] = 0;
	pa[0][0] = 0; pa[1][0] = 0;
	p = 0;
	t3 = 0;
	f = 0;
	f1 = 0;
	crx = 0;
	cry = 0;
	zov = 20;
	psu = 0,psd = 0,psl = 0,psr = 0,psc;
	Cube[0][1] = 10;
	Cube[0][0] = 10;
	c1 = 0;
}
//CHARACTER MOVEMENT FUNC:
void CrtMovement(int mode,int spd)
{
	bool bl = 0,bl1 = 0;int i,j,k;
	switch(mode){
	case 0:
	{
		for(i = 0;i < crtw;++i)
		if(map[(crty - 1) / z][(crtx + i) / z] == 2
		|| map[(crty - 1) / z][(crtx + i) / z] == 1)
    	{
    		bl = 1;
    		jmph = 0;
 	   	jmp = 0;
 	   	fine = 0;
 	   	break;
	    }
	    if(bl == 0)
	    {
    		for(i = 0;i < crtw;++i)
    	    for(j = 0;j <= spd / fps + 1;++j)
    	    if(map[(crty - 1 - j) / z][(crtx + i) / z] == 2
    	    || map[(crty - 1 - j) / z][(crtx + i) / z] == 1)
    		{
    	    	bl = 1;
    	    	goto label;
    		}
        	label:
    	    if(bl == 1)
    	    {
    	    	crty = ((crty - j) / z) * z;
    	    }
	    }
    	if(bl == 0)
		{ 
		    crty = crty - spd / fps;
	        jmph = jmph + spd / fps;
    	}
    	break;
	}
	case 1:
	{
		for(i = 0;i < crth;++i)
        if(map[(crty + i) / z][(crtx - 1) / z] == 2
        || map[(crty + i) / z][(crtx - 1) / z] == 1)
        {
        	for(k = 0;k < crth - z - 1;++k)
        	{
        		if(map[(crty + k) / z][(crtx - 1) / z] == 1 
        		|| map[(crty + k) / z][(crtx - 1) / z] == 2
        		|| ::k[SDLK_s])
        		{
        		    bl1 = 1;
        		    break;
        		}
        	}
        	if(bl1 == 0 && grd == 1)
        	{
        		crty -= z;
        	}
            bl = 1;
            rine = 0;
            fine = 0;
            if(bl1 == 1)
            {
                wt = 1;
                run = 0; 
            }
            jmp = 0;
            jmph = 0;
            if(wht == 0 && wt == 1)
            {
            	wht = 1;
            	fallh = 0;
            }
        	break;
        }
        if(bl == 0)
    	{
            for(i = 0;i < crth;++i)
            for(j = 0;j <= spd / fps + 1;++j)
            if(map[(crty + i) / z][(crtx - 1 - j) / z] == 2
            || map[(crty + i) / z][(crtx - 1 - j) / z] == 1)
            {   
                bl = 1;
                goto label1;
            }
            label1:
            if(bl == 1) 
            {
                crtx = ((crtx - j) / z) * z;
            }
        }
        if(bl == 0)
        {
            crtx = crtx - (spd / fps);
            wht = 0;
        }
        break;
	}
	case 2:
	{
		for(i = 0;i < crtw;++i)
    	if(map[(crty + crth) / z][(crtx + i) / z] == 2
    	|| map[(crty + crth) / z][(crtx + i) / z] == 1)
    	{
    		if((crty + crth) % z == 0)
    		{
    	        bl = 1;
    	        grd = 1;
    	        fallh = 0;
    	        fine = 0;
    	        rine = 0;
    	        jmph = 0;
    		}
    	    break;
    	}
    	if(bl == 0)
    	{
    	    for(i = 0;i < crtw;++i)
        	for(j = 0;j <= spd / fps + 1;++j)
        	if(map[(crty + crth + j) / z][(crtx + i) / z] == 2
        	|| map[(crty + crth + j) / z][(crtx + i) / z] == 1)
        	{
        		bl = 1;
        		goto label2;
    	    }
        	label2: if(bl == 1) 
        	{
        		crty = ((crty + crth + j) / z) * z - crth;
        	}
    	}
    	if(bl == 0) 
    	{
    	    crty = crty + spd / fps;
    	    if(jmp == 0) fallh = fallh + 1000 / fps;
    	    grd = 0;
    	}
    	break;
	}
	case 3:
	{
		for(i = 0;i < crth;++i)
        if(map[(crty + i) / z][(crtx + crtw) / z] == 2
        || map[(crty + i) / z][(crtx + crtw) / z] == 1)
        {
        	for(k = 0;k < crth - z - 1;++k)
        	{
        		if((map[(crty + k) / z][(crtx + crtw) / z] == 1 
        		|| map[(crty + k) / z][(crtx + crtw) / z] == 2)
        		|| ::k[SDLK_s])
        		{
        		    bl1 = 1;
        		    break;
        		}
        	}
        	if(bl1 == 0 && grd)
        	{
        		crty -= z;
        	}
            bl = 1;
            rine = 0;
            fine = 0;
            jmp = 0;
            jmph = 0;
            if(bl1 == 1)
            {
                run = 0;
                wt = 1;
            }
            if(wht == 0 && wt == 1)
            {
            	wht = 1;
            	fallh = 0;
            }
        	break;
        }
        if(bl == 0)
        {
            for(i = 0;i < crth;++i)
            for(j = 0;j <= spd / fps + 1;++j)
            if(map[(crty + i) / z][(crtx + crtw + j) / z] == 2
            || map[(crty + i) / z][(crtx + crtw + j) / z] == 1)
            {   
                bl = 1;
                goto label3;
            }
            label3:
            if(bl == 1) 
            crtx = ((crtx + crtw + j) / z) * z - crtw;
        }
        if(bl == 0)
        {
            crtx = crtx + spd / fps;
            wht = 0;
        }
	}}
}
//READFILE FUNC:
void readfile()
{
	int i,j;
	c = -1;
	ifstream fi("Demo/map/map.txt"),fi2("Demo/map/anomap.txt");
	fi >> w >> h;
	for(i = 0;i < h;++i)
	for(j = 0;j < w;++j)
	{
	    fi >> map[i][j];
	    fi2 >> anomap[i][j];
	}
	fi.close();
	fi2.close();
	for(i = 0;i < h;++i)
	for(j = 0;j < w;++j)
	{
		if(anomap[i][j] == 6)
		{
			c++;
			idbutton[c][0] = i;
			idbutton[c][1] = j;
		}
	}
}
//MAP GENERATOR FUNC:
/*
--> OBJs:
- 1: right pannel!
- 2: down pannel!
- 3: left pannel!
- 4: up pannel!
- 5: block!
- 6: button!
- 7: door closeed!
- 8 ... 10: door opening/closing!
- 11: door opened! 
- 12: Hide wall left up coner!
- 13: Hide wall right up coner!
- 14: Hide wall left down coner!
--> INVISIBLES:
- 1: Unportalable walls!
- 2: Portalable walls!
*/
void mapgenerator()
{
	SDL_Rect r;
	int i,j,k,l,m,mh,mw; bool a;
	for(i = - 2 * z - (crty - 350) % z;i < 720 + z * 2;i = i + z)
	for(j = - 2 * z - (crtx - 630) % z;j < 1280 + 2 * z;j = j + z)
	{
		if(crty - 350 +  i >= 0 && crtx - 630 + j >= 0)
		{
    		switch(anomap[(crty - 350 + i) / z][(crtx - 630 + j) / z])
    		{
        	    case 1:
        	    {
        	    	BlitSurface(pannelz,j,i - z,0,0,z,z * 3);
        	    	break;
        	    }
        	    case 2: 
        	    {
        	    	BlitSurface(pannelz,j - z,i,2 * z,0,z * 3,z);
        	    	break;
        	    }
        	    case 3:
        	    {
        	    	BlitSurface(pannelz,j,i - z,z,0,z,z * 3);
        	    	break;
        	    }
        	    case 4:
        	    {
        	    	BlitSurface(pannelz,j - z,i,z * 2,z,z * 3,z);
        	    	break;
        	    }
        	    case 5:
        	    {
        	    	BlitSurface(pannelz,j,i,2 * z,2 * z,z,z);
        	    	break;
        	    }
    		}
		}
	}
	for(i = - 2 * z - (crty - 350) % z;i < 720 + z * 2;i = i + z)
	for(j = - 2 * z - (crtx - 630) % z;j < 1280 + z * 2;j = j + z)
	{
		if(crty - 350 +  i >= 0 && crtx - 630 + j >= 0)
		{
			switch(anomap[(crty - 350 + i) / z][(crtx - 630 + j) / z])
    		{
    		    case 6:
        	    {
        	    	a = 0;
        	    	for(k = 0;k <= c;++k)
        	    	{
        	    	    if(idbutton[k][0] == (crty - 350 + i) / z 
        	    	    && idbutton[k][1] == (crtx - 630 + j) / z){
        	    	    if(crty + crth == idbutton[k][0] * z + z)
        	    	    for(l = crtx - z;l <= crtx + crtw + z;++l)
        	    	    if(l == idbutton[k][1] * z + z / 2)
        	    	    {
        	    	    	ba[k] = 1;
        	    	        a = 1;
        	    	        break;
        	    	    }
        	    	    if(a)
        	    	    break;
        	    	    else if(a == 0)
        	    	    for(l = 0;l <= c1;++l)
        	    	    if(Cube[l][0] + z == idbutton[k][0] * z)
        	    	    for(m = - z - z / 2;m < z * 3 + z / 2;++m)
        	    	    if(Cube[l][1] + m == idbutton[k][1] * z + z / 2)
        	    	    {
        	    	    	ba[k] = 1;
        	    	    	a = 1;
        	    	    	break;
        	    	    }if(a) break;}
        	    	}
        	    	if(a == 0)
        	    	BlitSurface(buttonz,j - 2 * z,i,0,0,z * 5,z * 2);
        	    	else if(a)
        	    	BlitSurface(buttonz,j - 2 * z,i,0,2 * z,z * 5,z * 2);
        	    	break;
        	    }
		    }
	    }
	}
	for(i = 0;i < h;++i)
    for(j = 0;j < w;++j)
    map2[i][j] = 0;
    spread(crty / z,crtx / z);
	for(i = 0;i < h;++i)
	for(j = 0;j < w;++j)
	{
		if(anomap[i][j] == 12)
		{
		    mw = 0;mh = 0;
		    do
		    {
			    ++mw;
		    }
		    while(anomap[i][j + mw] != 13);
		    do
		    {
			    ++mh;
		    }
		    while(anomap[i + mh][j] != 14);
		    if((i + mh > (crty - 350 - 2 * z) / z || j + mw > (crtx - 630 - 2 * z) / z
		    || i < (crty + 370 + 2 * z) / z || j < (crtx + 650 + 2 * z) / z)
		    && map2[i][j] != 1)
		    BlitSurface(wall,j * z - crtx + 630,i * z - crty + 350,0,0,
		    (mw + 1) * z,(mh + 1) * z);
		}
	}
	if(pa[0][0] == 1)
    {
    	if(pa[0][1] == 1 || pa[0][1] == 3)
    	{
            r.x = 630 - crtx + crtp[0][1] - crtw / 16;
            r.y = 350 - crty + crtp[0][0] - crth / 2;
            r.w = crtw / 8;
            r.h = crth;
    	}
    	if(pa[0][1] == 0 || pa[0][1] == 2)
    	{
    		r.x = 630 - crtx + crtp[0][1] - crth / 2;
    		r.y = 350 - crty + crtp[0][0] - crtw / 16;
    		r.w = crth;
    		r.h = crtw / 8;
    	}
        SDL_FillRect(scr,&r,u32a[9]);
    }
    if(pa[1][0] == 1)
    {
    	if(pa[1][1] == 1 || pa[1][1] == 3)
    	{
            r.x = 630 - crtx + crtp[1][1] - crtw / 16;
            r.y = 350 - crty + crtp[1][0] - crth / 2;
            r.w = crtw / 8;
            r.h = crth;
    	}
    	if(pa[1][1] == 0 || pa[1][1] == 2)
    	{
    		r.x = 630 - crtx + crtp[1][1] - crth / 2;
    		r.y = 350 - crty + crtp[1][0] - crtw / 16;
    		r.w = crth;
    		r.h = crtw / 8;
    	}
        SDL_FillRect(scr,&r,u32a[8]);
	}
}
//ZOOM FUNC:
void zoom(int zoom)
{
	int i,j;
	float f;
	SDL_Rect r;
	r.x = r.y = 0; r.w = r.h = 750;
	SDL_FillRect(crtz,&r,u32a[3]);
	f = zoom;
	f = f / z;
	crtx = roundf(crtx * f);
	crty = roundf(crty * f);
	for(i = 0;i <= c1;++i)
	{
		Cube[i][0] = roundf(Cube[i][0] * f);
		Cube[i][1] = roundf(Cube[i][1] * f);
	}
	crtp[0][0] = roundf(crtp[0][0] * f);
	crtp[0][1] = roundf(crtp[0][1] * f);
	crtp[1][0] = roundf(crtp[1][0] * f);
	crtp[1][1] = roundf(crtp[1][1] * f);
	pdp = roundf(pdp * f);
	z = zoom;
	crth = z * 5;
	crtw = z * 2;
	f = z;
	f = f / 10;
	for(i = 0;i < 50;++i)
	for(j = 0;j < 40;++j)
	{
		r.w = ceil(f);
		r.h = ceil(f);
		r.x = ceil(j * f);
		r.y = ceil(i * f);
		SDL_FillRect(crtz,&r,u32b[(i + crtcy) * 400 + j + crtcx]);
	}
	if(zoom != zz && dir != 8){
	r.x = r.y = 0; r.w = r.h = 750;
	SDL_FillRect(psaz,&r,u32a[3]);
	for(i = 0;i < 188;++i)
	for(j = 0;j < 188;++j)
	{
		r.w = ceil(f);
		r.h = ceil(f);
		r.x = ceil(j * f);
		r.y = ceil(i * f);
		SDL_FillRect(psaz,&r,u32c[i * 188 + j]);
	}
	zz = zoom;}
	if(zzz != zoom){
	for(i = 0;i < 30;++i)
	for(j = 0;j < 50;++j)
	{
		r.w = ceil(f);
		r.h = ceil(f);
		r.x = ceil(j * f);
		r.y = ceil(i * f);
		SDL_FillRect(pannelz,&r,u32d[i * 50 + j]);
	}
	for(i = 0;i < 40;++i)
	for(j = 0;j < 50;++j)
	{
		r.w = ceil(f);
		r.h = ceil(f);
		r.x = ceil(j * f);
		r.y = ceil(i * f);
		SDL_FillRect(buttonz,&r,u32e[i * 50 + j]);
	}
	for(i = 0;i < 50;++i)
	for(j = 0;j < 50;++j)
	{
		r.w = ceil(f);
		r.h = ceil(f);
		r.x = ceil(j * f);
		r.y = ceil(i * f);
		SDL_FillRect(doorz,&r,u32f[i * 50 + j]);
	}
	for(i = 0;i < 20;++i)
	for(j = 0;j < 20;++j)
	{
		r.w = ceil(f);
		r.h = ceil(f);
		r.x = ceil(j * f);
		r.y = ceil(i * f);
		SDL_FillRect(cubez,&r,u32g[i * 20 + j]);
	}
	zzz = zoom;}
}
//PORTAL PLACING FUNC:
void portal()
{
	int i,j,bl,lb,y,x;
	i = 0;
	if(p == 0)
	bl = 0;
	else if(p == 1)
	bl = 1;
	if(k[SDLK_UP] && pd[0] == 0)
	pd[0] = 1;
	if(k[SDLK_DOWN] && pd[2] == 0)
	pd[2] = 1;
	if(k[SDLK_LEFT] && pd[1] == 0)
	pd[1] = 1;
	if(k[SDLK_RIGHT] && pd[3] == 0)
	pd[3] = 1;
	if(pd[0] == 1 && k[SDLK_UP] == 0 && (k[SDLK_DOWN] || k[SDLK_RIGHT]
	|| k[SDLK_LEFT]))
	pd[0] = 0;
	if(pd[2] == 1 && k[SDLK_DOWN] == 0 && (k[SDLK_UP] || k[SDLK_RIGHT]
	|| k[SDLK_LEFT]))
	pd[2] = 0;
	if(pd[3] == 1 && k[SDLK_RIGHT] == 0 && (k[SDLK_UP] || k[SDLK_LEFT]
	|| k[SDLK_DOWN] || d != 1))
	pd[3] = 0;
	if(pd[1] == 1 && k[SDLK_LEFT] == 0 && (k[SDLK_UP] || k[SDLK_RIGHT]
	|| k[SDLK_DOWN] || d != 0))
	pd[1] = 0;
	if(pd[0] == 1 && k[SDLK_UP] == 0 && k[SDLK_DOWN] == 0
	&& k[SDLK_RIGHT] == 0 && k[SDLK_LEFT] == 0)
	{
		pd[0] = 0;
		psu = 1;
	}
	if(pd[2] == 1 && k[SDLK_UP] == 0 && k[SDLK_DOWN] == 0
	&& k[SDLK_RIGHT] == 0 && k[SDLK_LEFT] == 0)
	{
		pd[2] = 0;
		psd = 1;
	}
	if(pd[1] == 1 && k[SDLK_UP] == 0 && k[SDLK_DOWN] == 0
	&& k[SDLK_RIGHT] == 0 && k[SDLK_LEFT] == 0 && d == 0)
	{
		pd[1] = 0;
		psl = 1;
	}
	if(pd[3] == 1 && k[SDLK_UP] == 0 && k[SDLK_DOWN] == 0
	&& k[SDLK_RIGHT] == 0 && k[SDLK_LEFT] == 0 && d == 1)
	{
		pd[3] = 0;
		psr = 1;
	}
	if((psu == 1 && psd == 1) || (psl == 1 && psr == 1))
	goto label;
	else if(psu == 1 || psr == 1 || psl == 1 || psd == 1)
	{
		pa[bl][0] = 0;
    	crtp[bl][0] = crty;
    	crtp[bl][1] = crtx;
    	while(i < 100000)
    	{
    		i = i + 1;
            if(psu == 1)
            crtp[bl][0] = crtp[bl][0] - 1;
            if(psr == 1)
            crtp[bl][1] = crtp[bl][1] + 1;
            if(psl == 1)
            crtp[bl][1] = crtp[bl][1] - 1;
            if(psd == 1)
            crtp[bl][0] = crtp[bl][0] + 1;
            if(map[(crtp[bl][0] + crth / 2) / z][(crtp[bl][1] + crtw / 2) / z] == 2
            || map[(crtp[bl][0] + crth / 2) / z][(crtp[bl][1] + crtw / 2) / z] == 1)
            {
                crtp[bl][0] = crtp[bl][0] + crth / 2;
                crtp[bl][1] = crtp[bl][1] + crtw / 2;
                if(bl == 1)
                lb = 0;
                else if(bl == 0)
                lb = 1;
                pdp = i;
                pal = 255;
                psc = p;
                if(crtp[bl][0] >= crtp[lb][0] - crth
                && crtp[bl][0] <= crtp[lb][0] + crth
                && crtp[bl][1] >= crtp[lb][1] - crth
                && crtp[bl][1] <= crtp[lb][1] + crth
                && pa[lb][0] == 1)
                {
                    goto label;
                }
                break;
            }
	    }
	    x = 1;
	    y = 1;
        for(j = crtp[bl][0] - crth / 2 + 1;j <= crtp[bl][0] + crth / 2 - 1;++j)
        if(map[j / z][(crtp[bl][1] + 1) / z] == 2)
        x = 0;
        if(x == 1)
        for(j = crtp[bl][0] - crth / 2 + 1;j <= crtp[bl][0] + crth / 2 - 1;++j)
        if(map[j / z][(crtp[bl][1] - 1) / z] != 2)
        x = 0;
        if(x == 0)
        {
        	x = 1;
        	y = 3;
            for(j = crtp[bl][0] - crth / 2 + 1;j <= crtp[bl][0] + crth / 2 - 1;++j)
            if(map[j / z][(crtp[bl][1] - 1) / z] == 2)
            x = 0;
            if(x == 1)
            for(j = crtp[bl][0] - crth / 2 + 1;j <= crtp[bl][0] + crth / 2 - 1;++j)
            if(map[j / z][(crtp[bl][1] + 1) / z] != 2)
            x = 0;
        }
        if(x == 1)
        {
        	pa[bl][0] = 1;
        	pa[bl][1] = y;
        	goto label;
        }
        x = 1;
        y = 0;
        for(j = crtp[bl][1] - crth / 2 + 1;j <= crtp[bl][1] + crth / 2 - 1;++j)
        if(map[(crtp[bl][0] + 1) / z][j / z] == 2)
        x = 0;
        if(x == 1)
        for(j = crtp[bl][1] - crth/ 2 + 1;j <= crtp[bl][1] + crth / 2 - 1;++j)
        if(map[(crtp[bl][0] - 1) / z][j / z] != 2)
        x = 0;
        if(x == 0)
        {
            x = 1;
            y = 2;
            for(j = crtp[bl][1] - crth / 2 + 1;j <= crtp[bl][1] + crth / 2 - 1;++j)
            if(map[(crtp[bl][0] - 1) / z][j / z] == 2)
            x = 0;
            if(x == 1)
            for(j = crtp[bl][1] - crth / 2 + 1;j <= crtp[bl][1] + crth / 2 - 1;++j)
            if(map[(crtp[bl][0] + 1) / z][j / z] != 2)
            x = 0;
        }
        if(x == 1)
        {
        	pa[bl][0] = 1;
        	pa[bl][1] = y;
        	goto label;
        }
	}
	label:{}
}
//TELEPORT FUNC:
void teleport()
{
	int i,tlp = 0;
	for(i = crtp[0][0] - crth / 4;i <= crtp[0][0] + crth / 4;++i)
	if(crty + crth / 2 == i && crtx == crtp[0][1] + 1)
	{
		if(pa[0][1] == 1 && pa[0][0] == 1 && pa[1][0] == 1)
		{
	    	tlp = 1;
	    	goto label;
		}
	}
	for(i = crtp[0][0] - crth / 4;i <= crtp[0][0] + crth / 4;++i)
	if(crty + crth / 2 == i && crtx + crtw - 1 == crtp[0][1] - 1)
	{
		if(pa[0][1] == 3 && pa[0][0] == 1 && pa[1][0] == 1)
		{
		    tlp = 1;
		    goto label;
		}
	}
	for(i = crtp[0][1] - crth / 2;i <= crtp[0][1] + crth / 2;++i)
	if(crtx + crtw / 2 == i && crty == crtp[0][0] + 1)
	{
		if(pa[0][1] == 0 && pa[0][0] == 1 && pa[1][0] == 1)
		{
		    tlp = 1;
		    goto label;
		}
	}
	for(i = crtp[0][1] - crth / 2;i <= crtp[0][1] + crth / 2;++i)
	if(crtx + crtw / 2 == i && crty + crth - 1 == crtp[0][0] - 1)
	{
		if(pa[0][1] == 2 && pa[0][0] == 1 && pa[1][0] == 1)
		{
		    tlp = 1;
		    goto label;
		}
	}
	for(i = crtp[1][0] - crth / 4;i <= crtp[1][0] + crth / 4;++i)
	if(crty + crth / 2 == i && crtx == crtp[1][1] + 1)
	{
		if(pa[1][1] == 1 && pa[0][0] == 1 && pa[1][0] == 1)
		{
	    	tlp = 2;
	    	goto label;
		}
	}
	for(i = crtp[1][0] - crth / 4;i <= crtp[1][0] + crth / 4;++i)
	if(crty + crth / 2 == i && crtx + crtw - 1 == crtp[1][1] - 1)
	{
		if(pa[1][1] == 3 && pa[0][0] == 1 && pa[1][0] == 1)
		{
		    tlp = 2;
		    goto label;
		}
	}
	for(i = crtp[1][1] - crth / 2;i <= crtp[1][1] + crth / 2;++i)
	if(crtx + crtw / 2 == i && crty == crtp[1][0] + 1)
	{
		if(pa[1][1] == 0 && pa[0][0] == 1 && pa[1][0] == 1)
		{
		    tlp = 2;
		    goto label;
		}
	}
	for(i = crtp[1][1] - crth / 2;i <= crtp[1][1] + crth / 2;++i)
	if(crtx + crtw / 2 == i && crty + crth - 1 == crtp[1][0] - 1)
	{
		if(pa[1][1] == 2 && pa[0][0] == 1 && pa[1][0] == 1)
		{
		    tlp = 2;
		    goto label;
		}
	}
	label: 
	if(tlp == 1)
	{
		if(pa[0][1] == 2 && (pa[1][1] == 1 || pa[1][1] == 3))
		{
			fine = rine;
		    rine = fallh;
		    if(rine > 4000)
		    rine = 4000;
		    if(rine == 0)
		    rine = 250;
		    fallh = 0;
		}
		if(pa[0][1] == 2 && pa[1][1] == 2)
		{
			fine = fallh / 4 * 3;
			if(fine > 3000) fine = 3000;
		}
		if((pa[0][1] == 1 || pa[0][1] == 3) && pa[1][1] == 0)
		{
			fallh = rine;
			if(fallh > 1000) fallh = 1000;
		    rine = 0;
		}
		if(pa[0][1] == 0 && pa[1][1] == 0)
		{
			fallh = jmph;
		}
		if((pa[0][1] == 1 || pa[0][1] == 3) && pa[1][1] == 2)
		{
		    fine = rine;
		    rine = 0;
		    fallh = 0;
		}
		if(pa[0][1] == 0 && (pa[1][1] == 1 || pa[1][1] == 3))
		{
			fine = rine;
			rine = jmph * 10;
			jmph = 0;
			jmp = 0;
		}
    	if(pa[1][1] == 0)
    	{
    		crtx = crtp[1][1] - crtw / 2;
    		crty = crtp[1][0] + z / 5;
    		jmph = 0;
    		jmp = 0;
    	}
    	else if(pa[1][1] == 1)
    	{
	    	if(pa[0][1] == 1)
	    	if(d == 1)
	    	d = 0;
	    	else if(d == 0)
    		d = 1;
    		if(pa[0][1] == 2 || pa[0][1] == 0)
    		d = 1;
    		crty = crtp[1][0] - crth / 2;
    		crtx = crtp[1][1] + z / 5;
    	}
    	else if(pa[1][1] == 2)
    	{
    		crtx = crtp[1][1] - crtw / 2;
    		crty = crtp[1][0] - crth - z / 5;
    	}
    	else if(pa[1][1] == 3)
    	{
    		if(pa[0][1] == 3)
	    	if(d == 1)
    		d = 0;
    		else if(d == 0)
    		d = 1;
    		if(pa[0][1] == 0 || pa[0][1] == 2)
    		d = 0;
    		crtx = crtp[1][1] - crtw - z / 5;
    		crty = crtp[1][0] - crth / 2;
    	}
	}
	else if(tlp == 2)
	{
		if(pa[1][1] == 2 && (pa[0][1] == 1 || pa[0][1] == 3))
		{
			fine = rine;
		    rine = fallh;
		    if(rine > 4000)
		    rine = 4000;
		    if(rine == 0) 
		    rine = 250;
		    fallh = 0;
		}
		if(pa[0][1] == 2 && pa[1][1] == 2)
		{
			fine = fallh / 4 * 3;
			if(fine > 3000) fine = 3000;
		}
		if((pa[1][1] == 1 || pa[1][1] == 3) && pa[0][1] == 0)
		{
			fallh = rine;
			if(fallh > 1000) fallh = 1000;
		    rine = 0;
		}
		if(pa[0][1] == 0 && pa[1][1] == 0)
		{
			fallh = jmph;
		}
		if((pa[1][1] == 1 || pa[1][1] == 3) && pa[0][1] == 2)
		{
		    fine = rine;
		    rine = 0;
		    fallh = 0;
		}
		if(pa[1][1] == 0 && (pa[0][1] == 1 || pa[0][1] == 3))
		{
			fine = rine;
			rine = jmph * 10;
			jmph = 0;
			jmp = 0;
		}
    	if(pa[0][1] == 0)
    	{
    		crtx = crtp[0][1] - crtw / 2;
    		crty = crtp[0][0] + z / 5;
    		jmph = 0;
    		jmp = 0;
    	}
    	else if(pa[0][1] == 1)
    	{
	    	if(pa[1][1] == 1)
	    	if(d == 1)
	    	d = 0;
	    	else if(d == 0)
    		d = 1;
    		if(pa[1][1] == 2 || pa[1][1] == 0)
    		d = 1;
    		crty = crtp[0][0] - crth / 2;
    		crtx = crtp[0][1] + z / 5;
    	}
    	else if(pa[0][1] == 2)
    	{
    		crtx = crtp[0][1] - crtw / 2;
    		crty = crtp[0][0] - crth - z / 5;
    	}
    	else if(pa[0][1] == 3)
    	{
    		if(pa[1][1] == 3)
	    	if(d == 1)
    		d = 0;
    		else if(d == 0)
    		d = 1;
    		if(pa[1][1] == 0 || pa[1][1] == 2)
    		d = 0;
    		crtx = crtp[0][1] - crtw - z / 5;
    		crty = crtp[0][0] - crth / 2;
    	}
	}
}
//PHYSICAL JUMP, FALL N MOVEMENT EFFECTS:
void PJFnME()
{
	CrtMovement(2,0);
	if(fine == 0)
	if(grd == 0 && jmp == 0  && fallh >= 4000 && fine == 0)
	{
	    CrtMovement(2,80 * z); 
	    if(d == 1)
		CrtMovement(3,rine / 50 * z);
		else if(d == 0)
		CrtMovement(1,rine / 50 * z);
	}
	else if(grd == 0 && jmp == 0 && fallh < 4000 && fine == 0)
	{
	    CrtMovement(2,fallh / 50 * z);
	    if(d == 1)
		CrtMovement(3,rine / 50 * z);
		else if(d == 0)
		CrtMovement(1,rine / 50 * z);
	}
	if(k[SDLK_SPACE] && grd == 1 && SDL_GetTicks() - t3 >= 1000) 
	{
		jmp = 1;
		t3 = SDL_GetTicks();
	}
	if(jmp == 1)
	{
		CrtMovement(0,6 * z);
		if(d == 1)
		{
			if(rine > 0)
			{
				rine = 12 * 50;
		        CrtMovement(3,12* z);
			}
		}
		else if(d == 0)
		{
			if(rine > 0)
			{
				rine = 12 * 50;
		        CrtMovement(1,12 * z);
			}
		}
		if(jmph >= crth / 2)
		{
			fallh = 0;
			jmph = 0;
			jmp = 0;
		}
	}
	if(k[SDLK_w] && d == 1 && grd && run == 0)
	{
	    CrtMovement(3,6 * z);
	    rine = 6 * 50;
	    if(rh == 0)
	    rh = 1;
	    t4 = SDL_GetTicks();
	}
	else if(k[SDLK_w] && d == 0 && grd && run == 0)
	{
	    CrtMovement(1,6 * z);
	    rine = 6 * 50;
	    if(rh == 0)
	    rh = 1;
	    t4 = SDL_GetTicks();
	}
	if(rh == 1 && k[SDLK_w] == 0)
	rh = 2;
	if(rh == 2 && SDL_GetTicks() - t4 > 200)
	rh = 0;
	if(rh == 2 && k[SDLK_w])
	run = 1;
	if(run == 1 && d == 1 && grd)
	{
	    CrtMovement(3,18 * z);
	    rine = 18 * 50;
	}
	else if(run == 1 && d == 0 && grd)
	{
	    CrtMovement(1,18 * z);
	    rine = 18 * 50;
	}
	if(run == 1 && k[SDLK_w] == 0)
	run = 0;
	else if(k[SDLK_s] && d == 1 && grd)
	CrtMovement(1,6 * z);
	else if(k[SDLK_s] && d == 0 && grd)
	CrtMovement(3,6 * z);
	if(fine != 0)
	{
		fallh = 0;
		if(d == 1)
		CrtMovement(3,rine / 50 * z);
		else if(d == 0)
		CrtMovement(1,rine / 50 * z);
		CrtMovement(0,fine / 50 * z);
		if(fine - 2000 / fps >= 0)
		fine = fine - 2000 / fps;
		else 
		{
			fine = 0;
		}
	}
}
//CUBE'S PHYSIC EFFECT FUNC:
void CubeEffect()
{
	int i;
	for(i = 0;i <= c1;++i)
	{
		if(Cube[i][3] >= 4000){
	    CubeMovement(2,80 * z,i); }
	    else if(Cube[i][3] < 4000){
	    CubeMovement(2,Cube[i][3] / 50 * z,i);}  
	    if(cid != -1)
	    {
	    	if(d == 0) {Cube[cid][7] = rine; Cube[cid][9] = 0;}
	    	else if(d) {Cube[cid][9] = rine; Cube[cid][7] = 0;}
	    	Cube[cid][6] = fine;
	    	Cube[cid][3] = fallh;
	    }
	    if(Cube[i][7] > 0) CubeMovement(1,Cube[i][7] / 50 * z,i);
	    if(Cube[i][9] > 0) CubeMovement(3,Cube[i][9] / 50 * z,i);
	    if(Cube[i][6] > 0)
	    {
	    	Cube[i][3] = 0;
	        CubeMovement(0,Cube[i][6] / 50 * z,i);
	    }
	    if(Cube[i][6] - 2000 / fps >= 0) Cube[i][6] -= 2000 / fps;
	    else Cube[i][6] = 0;
	}
}
//CHARACTER'S ANIMATION FUNC:
void CrtAnimation()
{
	if(grd == 1 && k[SDLK_w] && run == 0)
	{
		scd = 0;
		++f1;
		crtcy = 0;
		if(f1 >= fps * 2)
		f1 = 0;
		crtcx = f1 / (fps * 2 / 8) * 80 + 80;
	    if(crtcx >= 400) 
		{
			crtcx = crtcx - 400;
		    crtcy = crtcy + 50;
		}
		if(d == 1) 
		{
			crtcx = crtcx + 40;
		    if(crtcx == 120 && crtcy == 0)
	        crx = 8;
	        else if(crtcx == 200 && crtcy == 0)
	        crx = 14;
	        else if(crtcx == 40 && crtcy == 50)
	        crx = 8;
	        else if(crtcx == 120 && crtcy == 50)
	        crx = 14;
		}
	}
	else if(grd == 1 && k[SDLK_s] && run == 0)
	{
		--f1;
		scd = 0;
		crtcy = 0;
		if(f1 < 0)
		f1 = fps * 2 - 1;
		crtcx = f1 / (fps * 2 / 8) * 80 + 80;
	    if(crtcx >= 400) 
		{
			crtcx = crtcx - 400;
		    crtcy = crtcy + 50;
		}
		if(d == 1) 
		{
			crtcx = crtcx + 40;
		    if(crtcx == 120 && crtcy == 0)
	        crx = 8;
	        else if(crtcx == 200 && crtcy == 0)
	        crx = 14;
	        else if(crtcx == 40 && crtcy == 50)
	        crx = 8;
	        else if(crtcx == 120 && crtcy == 50)
	        crx = 14;
		}
	}
	else if(run && grd == 1)
	{
		scd = 0;
		++f1;
		crtcy = 0;
		if(f1 >= fps / 3 * 2)
		f1 = 0;
		crtcx = f1 / (fps / 3 * 2 / 8) * 80 + 720;
		while(crtcx >= 400)
		{
			crtcx = crtcx - 400;
		    crtcy = crtcy + 50;
		}
		if(d == 1) 
		{
			crtcx = crtcx + 40;
		    if((crtcx == 360 && crtcy == 50) 
		    || (crtcx == 280 && crtcy == 100))
		    crx = 22;
		    else if((crtcx == 40 && crtcy == 100) 
		    || (crtcx == 360 && crtcy == 100))
		    crx = 10;
		    else if((crtcx == 120 && crtcy == 100) 
		    || (crtcx == 40 && crtcy == 150))
		    crx = 14;
		    else if((crtcx == 200 && crtcy == 100) 
		    || (crtcx == 120 && crtcy == 150))
		    crx = 20;
		}
	}
	else if((k[SDLK_s] == 0 && k[SDLK_w] == 0)
	&& (k[SDLK_UP] || k[SDLK_DOWN] || k[SDLK_LEFT] || k[SDLK_RIGHT]
	|| scd != 0)){
	if((((k[SDLK_RIGHT] && d) || (k[SDLK_LEFT] && d == 0))
	&& (p == 0 || p == 1) && k[SDLK_UP] == 0 && k[SDLK_DOWN] == 0)
	|| scd == 1)
	{
		scd = 1;
		if(k[SDLK_UP] || k[SDLK_DOWN] || k[SDLK_LEFT] || k[SDLK_RIGHT])
		f1 = 0;
		else ++f1;
		if(f1 >= fps / 2)
		{
			scd = 0;
		}
		crtcy = 150;
		if(d == 0)
		{
		    crtcx = 240;
    	}
    	else if(d)
    	{
    		crx = 12;
    	    crtcx = 280;
    	}
	}
	if(((p == 0 || p == 1) && k[SDLK_UP] == 1 && k[SDLK_DOWN] == 0)
	|| scd == 4)
	{
		scd = 4;
		if(k[SDLK_UP] || k[SDLK_DOWN] || k[SDLK_LEFT] || k[SDLK_RIGHT])
		f1 = 0;
		else ++f1;
		if(f1 >= fps / 2)
		{
			scd = 0;
		}
		crtcy = 200;
		if(d == 0)
		{
			crx = -4;
		    crtcx = 80;
    	}
    	else if(d)
    	{
    		crx = 12;
    	    crtcx = 120;
    	}
	}
	if(((p == 0 || p == 1) && k[SDLK_UP] == 0 && k[SDLK_DOWN] == 1)
	|| scd == 5)
	{
		scd = 5;
		if(k[SDLK_UP] || k[SDLK_DOWN] || k[SDLK_LEFT] || k[SDLK_RIGHT])
		f1 = 0;
		else ++f1;
		if(f1 >= fps / 2)
		{
			scd = 0;
		}
		crtcy = 200;
		if(d == 0)
		{
			crx = -4;
		    crtcx = 160;
    	}
    	else if(d)
    	{
    		crx = 12;
    	    crtcx = 200;
    	}
	}
    if((((k[SDLK_RIGHT] && d) || (k[SDLK_LEFT] && d == 0))
	&& (p == 0 || p == 1) && k[SDLK_UP] == 1 && k[SDLK_DOWN] == 0)
	|| scd == 2)
	{
		scd = 2;
		if(k[SDLK_UP] || k[SDLK_DOWN] || k[SDLK_LEFT] || k[SDLK_RIGHT])
		f1 = 0;
		else ++f1;
		if(f1 >= fps / 2)
		{
			scd = 0;
		}
		crtcy = 150;
		if(d == 0)
		{
			crx = 0;
		    crtcx = 320;
    	}
    	else if(d)
    	{
    		crx = 12;
    	    crtcx = 360;
    	}
	}
	if((((k[SDLK_RIGHT] && d) || (k[SDLK_LEFT] && d == 0))
	&& (p == 0 || p == 1) && k[SDLK_UP] == 0 && k[SDLK_DOWN] == 1)
	|| scd == 3)
	{
		scd = 3;
		if(k[SDLK_UP] || k[SDLK_DOWN] || k[SDLK_LEFT] || k[SDLK_RIGHT])
		f1 = 0;
		else ++f1;
		if(f1 >= fps / 2)
		{
			scd = 0;
		}
		crtcy = 200;
		if(d == 0)
		{
			crx = -4;
		    crtcx = 0;
    	}
    	else if(d)
    	{
    		crx = 12;
    	    crtcx = 40;
    	}
	}}
	else if(grd == 0)
	{
		++f1;
		if(f1 >= fps / 2) scd = 0;
		crtcx = 160;
		crtcy = 150;
		if(d)
		crtcx += 40;
		if(crtcx == 200)
		crx = 6;
	}
	if((scd == 0 && k[SDLK_w] == 0 && jmp == 0 && fine == 0 && fallh == 0
	&& k[SDLK_s] == 0 && grd && ((k[SDLK_UP] == 0 && k[SDLK_DOWN] == 0
	&& (k[SDLK_LEFT] == 0 || (k[SDLK_LEFT] && d)) 
	&& (k[SDLK_RIGHT] == 0 || (k[SDLK_RIGHT] && d == 0)) && p != 2)
	|| p == 2)) || cd == 1 || (wt && grd)) 
	{
		crx = cry = 0;
		crtcx = 0;
		crtcy = 0;
		f1 = 0;
		if(d == 1) crtcx = crtcx + 40;
	}
}
//PORTAL SHOOTING ANIMATION FUNC:
void PrtAnimation(int alpha,bool p)
{
    int i,j; float f = z; f = f / 10;
    if(psl == 1 && psd == 0 && psr == 0 && psu == 0) dir = 1;
    else if(psl == 0 && psd == 0 && psr == 1 && psu == 0) dir = 3;
    else if(psl == 0 && psd == 0 && psr == 0 && psu == 1) dir = 0;
    else if(psl == 0 && psd == 1 && psr == 0 && psu == 0) dir = 2;
    else if(psl == 0 && psd == 0 && psr == 1 && psu == 1) dir = 4;
    else if(psl == 0 && psd == 1 && psr == 1 && psu == 0) dir = 5;
    else if(psl == 1 && psd == 1 && psr == 0 && psu == 0) dir = 6;
    else if(psl == 1 && psd == 0 && psr == 0 && psu == 1) dir = 7;
    SDL_SetAlpha(psaz,SDL_SRCALPHA,alpha);
	if((((dir == 1))))
	{
		for(i = 0;i < (pdp - 10 * f) / (112 * f) - 1;++i)
		{
			if(p == 0)
			BlitSurface(psaz,630 - crtx + crtp[0][1] + (i * 112) * f,
			350 - crty + crtp[0][0] - 5 * f,0,0,112 * f,10 * f);
			else if(p == 1)
			BlitSurface(psaz,630 - crtx + crtp[1][1] + (i * 112) * f,
			350 - crty + crtp[1][0] - 5 * f,0,10 * f,112 * f,10 * f);
		}
		if(p == 0)
		BlitSurface(psaz,630 - crtx + crtp[0][1] + i * 112 * f,
		350 - crty + crtp[0][0] - 5 * f,0,0,fmod(pdp - 10 * f,112 * f),10 * f);
		else if(p == 1)
		BlitSurface(psaz,630 - crtx + crtp[1][1] + i * 112 * f,
		350 - crty + crtp[1][0] - 5 * f,0,10 * f,fmod(pdp - 10 * f,112 * f),10 * f);
	}
	else if((((dir == 3))))
	{
		for(i = 1;i <= (pdp - 10 * f) / (112 * f);++i)
		{
			if(p == 0)
			BlitSurface(psaz,630 - crtx + crtp[0][1] - i * 112 * f,
			350 - crty + crtp[0][0] - 5 * f,0,0,112 * f,10 * f);
			else if(p == 1)
			BlitSurface(psaz,630 - crtx + crtp[1][1] - i * 112 * f,
			350 - crty + crtp[1][0] - 5 * f,0,10 * f,112 * f,10 * f);
		}
		if(p == 0)
		BlitSurface(psaz,
		630 - crtx + crtp[0][1] - i * 112 * f + 112 * f - fmod((pdp - 10 * f),112 * f),
		350 - crty + crtp[0][0] - 5 * f,112 * f - fmod((pdp - 10 * f),112 * f),
		0,fmod(pdp - 10 * f,112 * f),10 * f);
		else if(p == 1)
	    BlitSurface(psaz,
		630 - crtx + crtp[1][1] - i * 112 * f + 112 * f - fmod((pdp - 10 * f),112 * f),
		350 - crty + crtp[1][0] - 5 * f,112 * f - fmod((pdp - 10 * f),112 * f),
		10 * f,fmod(pdp - 10 * f,112 * f),10 * f);
	}
	else if((((dir == 0))))
	{
		for(i = 0;i < (pdp - 25 * f) / (112 * f) - 1;++i)
		{
			if(p == 0)
			BlitSurface(psaz,630 - crtx + crtp[0][1] - 5 * f,
			350 - crty + crtp[0][0] + i * 112 * f,0,20 * f,10 * f,112 * f);
			else if(p == 1)
			BlitSurface(psaz,630 - crtx + crtp[1][1] - 5 * f,
			350 - crty + crtp[1][0] + i * 112 * f,10 * f,20 * f,10 * f,112 * f);
		}
		if(p == 0)
		BlitSurface(psaz,630 - crtx + crtp[0][1] - 5 * f,
		350 - crty + crtp[0][0] + i * 112 * f,0,20 * f,10 * f,
		fmod((pdp - 25 * f),112 * f));
		else if(p == 1)
		BlitSurface(psaz,630 - crtx + crtp[1][1] - 5 * f,
		350 - crty + crtp[1][0] + i * 112 * f,10 * f,20 * f,10 * f,
		fmod((pdp - 25 * f),112 * f));
	}
	else if((((dir == 2))))
	{
		for(i = 1;i <= (pdp - 25 * f) / (112 * f);++i)
		{
			if(p == 0)
			BlitSurface(psaz,630 - crtx + crtp[0][1] - 5 * f,
			350 - crty + crtp[0][0] - i * 112 * f,0,20 * f,10 * f,112 * f);
			else if(p == 1)
			BlitSurface(psaz,630 - crtx + crtp[1][1] - 5 * f,
			350 - crty + crtp[1][0] - i * 112 * f,10 * f,20 * f,10 * f,112 * f);
		}
		if(p == 0)
		BlitSurface(psaz,630 - crtx + crtp[0][1] - 5 * f,
		350 - crty + crtp[0][0] - i * 112 * f + 112 * f - fmod(pdp - 25 * f,
		112 * f),0,20 * f + 112 * f - fmod(pdp - 25 * f,112 * f),10 * f,
		fmod(pdp - 25 * f,112 * f));
		else if(p == 1)
		BlitSurface(psaz,630 - crtx + crtp[1][1] - 5 * f,
		350 - crty + crtp[1][0] - i * 112 * f + 112 * f - fmod(pdp - 25 * f,
		112 * f),10 * f,20 * f + 112 * f - fmod(pdp - 25 * f,112 * f),10 * f,
		fmod(pdp - 25 * f,112 * f));
	}
	else if((((dir == 4))))
	{
		for(i = 1,j = 2;i < (pdp - 10 * f) / (78 * f) - 1;++i,++j)
		{
			if(p == 0) BlitSurface(psaz,630 - crtx + crtp[0][1] - j * 78 * f,
			350 - 6 * f - crty + crtp[0][0] + i * 78 * f,20 * f,20 * f,84 * f,84 * f);
			else if(p == 1) BlitSurface(psaz,630 - crtx + crtp[1][1] - j * 78 * f,
			350 - 6 * f - crty + crtp[1][0] + i * 78 * f,104 * f,20 * f,84 * f,84 * f);
		}
		if(p == 0)
		{
			if(pdp - 10 * f <= 78 * f)
			{
				--i;--j;
			}
			if(pdp - 10 * f > 78 * f)
			BlitSurface(psaz,630 - crtx + crtp[0][1] - 78 * f,
			350 - 6 * f - crty + crtp[0][0],20 * f,20 * f,84 * f,84 * f);
			BlitSurface(psaz,
			630 - crtx + crtp[0][1] - j * 78 * f + 84 * f - fmod((pdp - 10 * f),78 * f),
			350 - 6 * f - crty + crtp[0][0] + i * 78 * f,
			20 * f + 84 * f - fmod((pdp - 10 * f),78 * f),20 * f,
			fmod((pdp - 10 * f),78 * f),fmod((pdp - 10 * f),78 * f));
		}
		else if(p == 1)
		{
			if(pdp - 10 * f <= 78 * f)
			{
				--i;--j;
			}
			if(pdp - 10 * f > 78 * f)
			BlitSurface(psaz,630 - crtx + crtp[1][1] - 78 * f,
			350 - 6 * f - crty + crtp[1][0],104 * f,20 * f,84 * f,84 * f);
			BlitSurface(psaz,
			630 - crtx + crtp[1][1] - j * 78 * f + 84 * f - fmod((pdp - 10 * f),78 * f),
			350 - 6 * f - crty + crtp[1][0] + i * 78 * f,
			104 * f + 84 * f - fmod((pdp - 10 * f),78 * f),20 * f,
			fmod((pdp - 10 * f),78 * f),fmod((pdp - 10 * f),78 * f));
		}
	}
	else if((((dir == 5))))
	{
		for(i = 2,j = 2;i <= (pdp - 10 * f) / (78 * f);++i,++j)
		{
			if(p == 0) BlitSurface(psaz,630 - crtx + crtp[0][1] - j * 78 * f,
			350 - crty + crtp[0][0] - i * 78 * f,20 * f,104 * f,84 * f,84 * f);
			else if(p == 1) BlitSurface(psaz,630 - crtx + crtp[1][1] - j * 78 * f,
			350 - crty + crtp[1][0] - i * 78 * f,104 * f,104 * f,84 * f,84 * f);
		}
		if(p == 0)
		{
			if(pdp - 10 * f <= 78 * f)
			{
				--i;--j;
			}
			if(pdp - 10 * f > 78 * f)
			BlitSurface(psaz,630 - crtx + crtp[0][1] - 78 * f,
			350 - crty + crtp[0][0] - 78 * f,20 * f,104 * f,84 * f,84 * f);
			BlitSurface(psaz,
			630 - crtx + crtp[0][1] - j * 78 * f + 84 * f - fmod(pdp - 10 * f,78 * f),
			350 - crty + crtp[0][0] - i * 78 * f + 84 * f - fmod(pdp - 10 * f,78 * f),
			20 * f + 84 * f - fmod(pdp - 10 * f,78 * f),
			104 * f + 84 * f - fmod(pdp - 10 * f,78 * f),
			fmod(pdp - 10 * f,78 * f),fmod(pdp - 10 * f,78 * f));
		}
		else if(p == 1)
		{
			if(pdp - 10 * f <= 78 * f)
			{
				--i;--j;
			}
			if(pdp - 10 * f > 78 * f)
			BlitSurface(psaz,630 - crtx + crtp[1][1] - 78 * f,
			350 - crty + crtp[1][0] - 78 * f,104 * f,104 * f,84 * f,84 * f);
			BlitSurface(psaz,
			630 - crtx + crtp[1][1] - j * 78 * f + 84 * f - fmod(pdp - 10 * f,78 * f),
			350 - crty + crtp[1][0] - i * 78 * f + 84 * f - fmod(pdp - 10 * f,78 * f),
			104 * f + 84 * f - fmod(pdp - 10 * f,78 * f),
			104 * f + 84 * f - fmod(pdp - 10 * f,78 * f),
			fmod(pdp - 10 * f,78 * f),fmod(pdp - 10 * f,78 * f));
		}
	}
	else if((((dir == 6))))
	{
		for(i = 2,j = 1;i <= (pdp - 10 * f) / (78 * f);++i,++j)
		{
			if(p == 0) BlitSurface(psaz,630 - 6 * f - crtx + crtp[0][1] + j * 78 * f,
			350 - crty + crtp[0][0] - i * 78 * f,20 * f,20 * f,84 * f,84 * f);
			else if(p == 1) BlitSurface(psaz,630 - 6 * f - crtx + crtp[1][1] + j * 78 * f,
			350 - crty + crtp[1][0] - i * 78 * f,104 * f,20 * f,84 * f,84 * f);
		}
		if(p == 0)
		{
			if(pdp - 10 * f <= 78 * f)
			{
				--i;--j;
			}
			if(pdp - 10 * f > 78 * f)
			BlitSurface(psaz,630 - 6 * f - crtx + crtp[0][1],
			350 - crty + crtp[0][0] - 78 * f,20 * f,20 * f,84 * f,84 * f);
			BlitSurface(psaz,630 - 6 * f - crtx + crtp[0][1] + j * 78 * f,
			350 - crty + crtp[0][0] - i * 78 * f + 84 * f - fmod(pdp - 10 * f,78 * f),
			20 * f,20 * f + 84 * f - fmod(pdp - 10 * f,78 * f),
			fmod(pdp - 10 * f,78 * f),fmod(pdp - 10 * f,78 * f));
		}
		else if(p == 1)
		{
			if(pdp - 10 * f <= 78 * f)
			{
				--i;--j;
			}
			if(pdp - 10 * f > 78 * f)
			BlitSurface(psaz,630 - 6 * f - crtx + crtp[1][1],
			350 - crty + crtp[1][0] - 78 * f,104 * f,20* f,84 * f,84 * f);
			BlitSurface(psaz,630 - 6 * f - crtx + crtp[1][1] + j * 78 * f,
			350 - crty + crtp[1][0] - i * 78 * f + 84 * f - fmod(pdp - 10 * f,78 * f),
			104 * f,20 * f + 84 * f - fmod(pdp - 10 * f,78 * f),
			fmod(pdp - 10 * f,78 * f),fmod(pdp - 10 * f,78 * f));
		}
	}
	else if((((dir == 7))))
	{
		for(i = 1,j = 1;i < (pdp - 10 * f) / (78 * f) - 1;++i,++j)
		{
			if(p == 0) BlitSurface(psaz,630 - 6 * f - crtx + crtp[0][1] + j * 78 * f,
			350 - 6 * f - crty + crtp[0][0] + i * 78 * f,20 * f,104 * f,84 * f,84 * f);
			else if(p == 1) BlitSurface(psaz,630 - 6 * f - crtx + crtp[1][1] + j * 78 * f,
			350 - 6 * f - crty + crtp[1][0] + i * 78 * f,104 * f,104 * f,84 * f,84 * f);
		}
		if(p == 0)
		{
			if(pdp - 10 * f <= 78 * f)
			{
				--i;--j;
			}
			if(pdp - 10 * f > 78 * f)
			BlitSurface(psaz,630 - 6 * f - crtx + crtp[0][1],
			350 - 6 * f - crty + crtp[0][0],20 * f,104 * f,84 * f,84 * f);
			BlitSurface(psaz,
			630 - 6 * f - crtx + crtp[0][1] + j * 78 * f,
			350 - 6 * f - crty + crtp[0][0] + i * 78 * f,20 * f,104 * f,
			fmod(pdp - 10 * f,78 * f),fmod(pdp - 10 * f,78 * f));
		}
		else if(p == 1)
		{
			if(pdp - 10 * f <= 78 * f)
			{
				--i;--j;
			}
			if(pdp - 10 * f > 78 * f)
			BlitSurface(psaz,630 - 6 * f - crtx + crtp[1][1],
			350 - 6 * f - crty + crtp[1][0],104 * f,104 * f,84 * f,84 * f);
			BlitSurface(psaz,
			630 - 6 * f - crtx + crtp[1][1] + j * 78 * f,
			350 - 6 * f - crty + crtp[1][0] + i * 78 * f,104 * f,104 * f,
			fmod(pdp - 10 * f,78 * f),fmod(pdp - 10 * f,78 * f));
		}
	}
}
//WALL GENERATOR FUNC:
void wallgenerator()
{
	int i,j;
	for(i = - 2 * z - (crty - 350) % z;i < 720 + z * 2;i = i + z)
	for(j = - 2 * z - (crtx - 630) % z;j < 1280 + z * 2;j = j + z)
	{
		if(crty - 350 +  i >= 0 && crtx - 630 + j >= 0)
		{
    		switch(anomap[(crty - 350 + i) / z][(crtx - 630 + j) / z])
    		{
    			case 7:
        	    {
        	    	BlitSurface(doorz,j,i - z * 2,0,0,z,5 * z);
        	    	break;
        	    }
        	    case 8:
        	    {
        	    	BlitSurface(doorz,j,i - z * 2,z,0,z,5 * z);
        	    	break;
        	    }
        	    case 9:
        	    {
        	    	BlitSurface(doorz,j,i - z * 2,z * 2,0,z,5 * z);
        	    	break;
        	    }
        	    case 10:
        	    {
        	    	BlitSurface(doorz,j,i - z * 2,z * 3,0,z,5 * z);
        	    	break;
        	    }
        	    case 11:
        	    {
        	    	BlitSurface(doorz,j,i - z * 2,z * 4,0,z,5 * z);
        	    	break;
        	    }
    		}
		}
	}
	for(i = 0;i <= c1;++i)
	{
		BlitSurface(cubez,630 - crtx + Cube[i][1],
		350 - crty + Cube[i][0],0,0,z * 2,z * 2);
	}
}
//BUTTON EFFECTS FUNC:
void UseButton()
{
	if(ba[0] == 1)
	OpenDoor(59,16);
	else if(ba[0] == 0)
	CloseDoor(59,16,2);
}
//DEATIVE PORTAL FUNC:
void De_ativePortal()
{
	int i,j; bool c1,c;
	for(i = 0;i < 2;++i)
	if(pa[i][0] == 1)
	{
		if(pa[i][1] == 1 || pa[i][1] == 3){
		c1 = c = 1;
		for(j = 0;j < crth;++j)
		if(map[(crtp[i][0] - crth / 2 + j) / z][(crtp[i][1] + 1) / z] != 2
		|| map[(crtp[i][0] - crth / 2 + j) / z][(crtp[i][1] - 1) / z] != 0)
		c = 0;
		for(j = 0;j < crth;++j)
		if(map[(crtp[i][0] - crth / 2 + j) / z][(crtp[i][1] - 1) / z] != 2
		|| map[(crtp[i][0] - crth / 2 + j) / z][(crtp[i][1] + 1) / z] != 0)
		c1 = 0;
		if(c == 0 && c1 == 0)
		pa[i][0] = 0;}
	}
}
//CUBES'S MOVEMENT FUNC:
void CubeMovement(int mode,int spd,int id)
{
	bool bl = 0,bl1 = 0;int i,j,k;
	switch(mode){
	case 0:
	{
		for(i = 0;i < z * 2;++i)
		if(map[(Cube[id][0] - 1) / z][(Cube[id][1] + i) / z] == 2
		|| map[(Cube[id][0] - 1) / z][(Cube[id][1] + i) / z] == 1)
    	{
    		bl = 1;
	    }
	    if(bl == 0)
	    {
    		for(i = 0;i < z * 2;++i)
    	    for(j = 0;j <= spd / fps + 1;++j)
    	    if(map[(Cube[id][0] - 1 - j) / z][(Cube[id][1] + i) / z] == 2
    	    || map[(Cube[id][0] - 1 - j) / z][(Cube[id][1] + i) / z] == 1)
    		{
    	    	bl = 1;
    	    	goto label;
    		}
        	label:
    	    if(bl == 1)
    	    {
    	    	crty = ((z * 2 - j) / z) * z;
    	    }
	    }
    	if(bl == 0)
		{ 
		    Cube[id][0] = Cube[id][0] - spd / fps;
    	}
    	break;
	}
	case 1:
	{
		for(i = 0;i < z * 2;++i)
        if(map[(Cube[id][0] + i) / z][(Cube[id][1] - 1) / z] == 2
        || map[(Cube[id][0] + i) / z][(Cube[id][1] - 1) / z] == 1)
        {
            bl = 1;
        	break;
        }
        if(bl == 0)
    	{
            for(i = 0;i < z * 2;++i)
            for(j = 0;j <= spd / fps + 1;++j)
            if(map[(Cube[id][0] + i) / z][(Cube[id][1] - 1 - j) / z] == 2
            || map[(Cube[id][0] + i) / z][(Cube[id][1] - 1 - j) / z] == 1)
            {   
                bl = 1;
                goto label1;
            }
            label1:
            if(bl == 1) 
            {
                Cube[id][1] = ((Cube[id][1] - j) / z) * z;
            }
        }
        if(bl == 0)
        {
            Cube[id][1] = Cube[id][1] - (spd / fps);
        }
        break;
	}
	case 2:
	{
		for(i = 0;i < z * 2;++i)
    	if(map[(Cube[id][0] + z * 2) / z][(Cube[id][1] + i) / z] == 2
    	|| map[(Cube[id][0] + z * 2) / z][(Cube[id][1] + i) / z] == 1)
    	{
    		if((Cube[id][0] + z * 2) % z == 0)
    		{
    	        bl = 1;
    	        Cube[id][2] = 1;
    	        Cube[id][3] = 0;
    	        Cube[id][7] = Cube[id][9] = 0;
    		}
    	    break;
    	}
    	if(bl == 0)
    	{
    	    for(i = 0;i < z * 2;++i)
        	for(j = 0;j <= spd / fps + 1;++j)
        	if(map[(Cube[id][0] + z * 2 + j) / z][(Cube[id][1] + i) / z] == 2
        	|| map[(Cube[id][0] + z * 2 + j) / z][(Cube[id][1] + i) / z] == 1)
        	{
        		bl = 1;
        		goto label2;
    	    }
        	label2: if(bl == 1) 
        	{
        		Cube[id][0] = ((Cube[id][0] + z * 2 + j) / z) * z - z * 2;
        	}
    	}
    	if(bl == 0) 
    	{
    	    Cube[id][0] = Cube[id][0] + spd / fps;
    	    Cube[id][3] += 2000 / fps;
    	    Cube[id][2] = 0;
    	}
    	break;
	}
	case 3:
	{
		for(i = 0;i < z * 2;++i)
        if(map[(Cube[id][0] + i) / z][(Cube[id][1] + z * 2) / z] == 2
        || map[(Cube[id][0] + i) / z][(Cube[id][1] + z * 2) / z] == 1)
        {
            bl = 1;
        	break;
        }
        if(bl == 0)
        {
            for(i = 0;i < z * 2;++i)
            for(j = 0;j <= spd / fps + 1;++j)
            if(map[(Cube[id][0] + i) / z][(Cube[id][1] + z * 2 + j) / z] == 2
            || map[(Cube[id][0] + i) / z][(Cube[id][1] + z * 2 + j) / z] == 1)
            {   
                bl = 1;
                goto label3;
            }
            label3:
            if(bl == 1) 
            Cube[id][1] = ((Cube[id][1] + z * 2 + j) / z) * z - z * 2;
        }
        if(bl == 0)
        {
            Cube[id][1] = Cube[id][1] + spd / fps;
        }
	}}
}
//PICKUP FUNC:
void pickup()
{
	int i,j; bool bl = 0;
	if(d == 0)
	{
		for(i = c1;i >= 0;--i)
		if(Cube[i][0] + 2 * z >= crty + 2 * z
		&& Cube[i][0] <= crty + crth - 2 * z
		&& Cube[i][1] + 2 * z >= crtx - crtw - 2 * z
		&& Cube[i][1] <= crtx - crtw)
		{
			if(cid == -1)
			cid = i;
			bl = 1;
			break;
		}
		for(i = 0;i < 2 * z;++i)
		for(j = 0;j < 2 * z;++j)
		{
			if((map[(crty + crth / 2 - z + i) / z][(crtx - crtw - z + j) / z] == 1
			|| map[(crty + crth / 2 - z + i) / z][(crtx - crtw - z + j) / z] == 2)
			&& cid != -1)
			{
				cid = -1;
				break;
			}
		}
		Cube[cid][0] = crty + crth / 2 - z;
		Cube[cid][1] = crtx - crtw - z;
		Cube[cid][3] = 0;
	}
	if(d == 1)
	{
		for(i = c1;i >= 0;--i)
		if(Cube[i][0] + 2 * z >= crty + 2 * z
		&& Cube[i][0] <= crty + crth - 2 * z
		&& Cube[i][1] <= crtx + 2 * crtw + 2 * z
		&& Cube[i][1] + 2 * z >= crtx + 2 * crtw)
		{
			if(cid == -1)
			cid = i;
			bl = 1;
			break;
		}
		for(i = 0;i < 2 * z;++i)
		for(j = 0;j < 2 * z;++j)
		{
			if((map[(crty + crth / 2 - z + i) / z][(crtx + 2 * crtw - z + j) / z] == 1
			|| map[(crty + crth / 2 - z + i) / z][(crtx + 2 * crtw - z + j) / z] == 2)
			&& cid != -1)
			{
				cid = -1;
				break;
			}
		}
		Cube[cid][0] = crty + crth / 2 - z;
		Cube[cid][1] = crtx + 2 * crtw - z;
		Cube[cid][3] = 0;
	}
	else if(bl == 0)
	cid = -1;
}
//CUBE TELEPORTING FUNC
void CubeTlp()
{
	int i,id,tlp = 0;
	for(id = 0;id <= c1;++id){
	tlp = 0;
	for(i = crtp[0][0] - crth  / 4;i <= crtp[0][0] + crth / 4;++i)
	if(Cube[id][0] + z == i && Cube[id][1] == crtp[0][1] + 1)
	{
		if(pa[0][1] == 1 && pa[0][0] == 1 && pa[1][0] == 1)
		{
	    	tlp = 1;
	    	goto label;
		}
	}
	for(i = crtp[0][0] - crth / 4;i <= crtp[0][0] + crth / 4;++i)
	if(Cube[id][0] + z == i && Cube[id][1] + z * 2 - 1 == crtp[0][1] - 1)
	{
		if(pa[0][1] == 3 && pa[0][0] == 1 && pa[1][0] == 1)
		{
		    tlp = 1;
		    goto label;
		}
	}
	for(i = crtp[0][1] - crth / 2;i <= crtp[0][1] + crth / 2;++i)
	if(Cube[id][1] + z == i && Cube[id][0] == crtp[0][0] + 1)
	{
		if(pa[0][1] == 0 && pa[0][0] == 1 && pa[1][0] == 1)
		{
		    tlp = 1;
		    goto label;
		}
	}
	for(i = crtp[0][1] - crth / 2;i <= crtp[0][1] + crth / 2;++i)
	if(Cube[id][1] + z == i && Cube[id][0] + z * 2 - 1 == crtp[0][0] - 1)
	{
		if(pa[0][1] == 2 && pa[0][0] == 1 && pa[1][0] == 1)
		{
		    tlp = 1;
		    goto label;
		}
	}
	for(i = crtp[1][0] - crth / 4;i <= crtp[1][0] + crth / 4;++i)
	if(Cube[id][0] + z == i && Cube[id][1] == crtp[1][1] + 1)
	{
		if(pa[1][1] == 1 && pa[0][0] == 1 && pa[1][0] == 1)
		{
	    	tlp = 2;
	    	goto label;
		}
	}
	for(i = crtp[1][0] - crth / 4;i <= crtp[1][0] + crth / 4;++i)
	if(Cube[id][0] + z == i && Cube[id][1] + z * 2 - 1 == crtp[1][1] - 1)
	{
		if(pa[1][1] == 3 && pa[0][0] == 1 && pa[1][0] == 1)
		{
		    tlp = 2;
		    goto label;
		}
	}
	for(i = crtp[1][1] - crth / 2;i <= crtp[1][1] + crth / 2;++i)
	if(Cube[id][1] + z == i && Cube[id][0] == crtp[1][0] + 1)
	{
		if(pa[1][1] == 0 && pa[0][0] == 1 && pa[1][0] == 1)
		{
		    tlp = 2;
		    goto label;
		}
	}
	for(i = crtp[1][1] - crth / 2;i <= crtp[1][1] + crth / 2;++i)
	if(Cube[id][1] + z == i && Cube[id][0] + z * 2 - 1 == crtp[1][0] - 1)
	{
		if(pa[1][1] == 2 && pa[0][0] == 1 && pa[1][0] == 1)
		{
		    tlp = 2;
		    goto label;
		}
	}
	label: 
	if(tlp == 1)
	{
		if(pa[0][1] == 2 && (pa[1][1] == 1 || pa[1][1] == 3))
		{
			if(Cube[id][7] == 0 && Cube[id][9] != 0)
			Cube[id][6] = Cube[id][9];
			else if(Cube[id][9] == 0 && Cube[id][7] != 0)
			Cube[id][6] = Cube[id][7];
			else Cube[id][6] = 0;
		    if(pa[1][1] == 3) 
		    {
		    	Cube[id][7] = Cube[id][3];
		    	Cube[id][9] = 0;
		    }
		    else if(pa[1][1] == 1) 
		    {
		    	Cube[id][9] = Cube[id][3];
		    	Cube[id][7] = 0;
		    }
		    if(Cube[id][7] > 4000)
		    Cube[id][7] = 4000;
		    if(Cube[id][9] > 4000)
		    Cube[id][9] = 4000;
		    Cube[id][3] = 0;
		}
		if(pa[0][1] == 2 && pa[1][1] == 2)
		{
			Cube[id][6] = Cube[id][3] / 4 * 3;
			if(Cube[id][6] > 3000) Cube[id][6] = 3000;
		}
		if((pa[0][1] == 1 || pa[0][1] == 3) && pa[1][1] == 0)
		{
			if(Cube[id][7] == 0 && Cube[id][9] != 0)
			Cube[id][3] = Cube[id][9];
			else if(Cube[id][9] == 0 && Cube[id][7] != 0)
			Cube[id][3] = Cube[id][7];
			if(Cube[id][3] > 1000) Cube[id][3] = 1000;
		    Cube[id][7] = 0; Cube[id][9] = 0;
		}
		if(pa[0][1] == 0 && pa[1][1] == 0)
		{
			Cube[id][3] = Cube[id][6];
		}
		if((pa[0][1] == 1 || pa[0][1] == 3) && pa[1][1] == 2)
		{
		    if(Cube[id][7] == 0 && Cube[id][9] != 0)
			Cube[id][6] = Cube[id][9];
			else if(Cube[id][9] == 0 && Cube[id][7] != 0)
			Cube[id][6] = Cube[id][7];
		    Cube[id][7] = 0; Cube[id][9] = 0;
		    Cube[id][3] = 0;
		}
		if(pa[0][1] == 0 && (pa[1][1] == 1 || pa[1][1] == 3))
		{
			if(Cube[id][7] == 0 && Cube[id][9] != 0)
			Cube[id][6] = Cube[id][9];
			else if(Cube[id][9] == 0 && Cube[id][7] != 0)
			Cube[id][6] = Cube[id][7];
			if(pa[0][1] == 1) Cube[id][9] = Cube[id][6];
			if(pa[0][1] == 3) Cube[id][7] = Cube[id][6];
		}
    	if(pa[1][1] == 0)
    	{
    		Cube[id][1] = crtp[1][1] - z;
    		Cube[id][0] = crtp[1][0] + z / 5;
    	}
    	else if(pa[1][1] == 1)
    	{
	    	if(pa[0][1] == 1)
	    	{
	    		Cube[id][9] = Cube[id][7];
	    		Cube[id][7] = 0;
	    	}
    		Cube[id][0] = crtp[1][0] - z;
    		Cube[id][1] = crtp[1][1] + z / 5;
    	}
    	else if(pa[1][1] == 2)
    	{
    		Cube[id][1] = crtp[1][1] - z;
    		Cube[id][0] = crtp[1][0] - 2 * z - z / 5;
    	}
    	else if(pa[1][1] == 3)
    	{
    		if(pa[0][1] == 3)
	    	{
	    		Cube[id][7] = Cube[id][9];
	    		Cube[id][9] = 0;
	    	}
    		Cube[id][1] = crtp[1][1] - 2 * z - z / 5;
    		Cube[id][0] = crtp[1][0] - z;
    	}
	}
	else if(tlp == 2)
	{
		if(pa[1][1] == 2 && (pa[0][1] == 1 || pa[0][1] == 3))
		{
			if(Cube[id][7] == 0 && Cube[id][9] != 0)
			Cube[id][6] = Cube[id][9];
			else if(Cube[id][9] == 0 && Cube[id][7] != 0)
			Cube[id][6] = Cube[id][7];
			else Cube[id][6] = 0;
		    if(pa[0][1] == 3) 
		    {
		    	Cube[id][7] = Cube[id][3];
		    	Cube[id][9] = 0;
		    }
		    else if(pa[0][1] == 1)
		    {
		    	Cube[id][9] = Cube[id][3];
		    	Cube[id][7] = 0;
		    }
		    if(Cube[id][7] > 4000)
		    Cube[id][7] = 4000;
		    if(Cube[id][9] > 4000)
		    Cube[id][9] = 4000;
		    Cube[id][3] = 0;
		}
		if(pa[0][1] == 2 && pa[1][1] == 2)
		{
			Cube[id][6] = Cube[id][3] / 4 * 3;
			if(Cube[id][6] > 3000) Cube[id][6] = 3000;
		}
		if((pa[1][1] == 1 || pa[1][1] == 3) && pa[0][1] == 0)
		{
			if(Cube[id][7] == 0 && Cube[id][9] != 0)
			Cube[id][3] = Cube[id][9];
			else if(Cube[id][9] == 0 && Cube[id][7] != 0)
			Cube[id][3] = Cube[id][7];
			if(Cube[id][3] > 1000) Cube[id][3] = 1000;
		    Cube[id][7] = 0; Cube[id][9] = 0;
		}
		if(pa[0][1] == 0 && pa[1][1] == 0)
		{
			Cube[id][3] = Cube[id][6];
		}
		if((pa[1][1] == 1 || pa[1][1] == 3) && pa[0][1] == 2)
		{
		    if(Cube[id][7] == 0 && Cube[id][9] != 0)
			Cube[id][6] = Cube[id][9];
			else if(Cube[id][9] == 0 && Cube[id][7] != 0)
			Cube[id][6] = Cube[id][7];
			Cube[id][7] = 0; Cube[id][9] = 0;
			Cube[id][3] = 0;
		}
		if(pa[1][1] == 0 && (pa[0][1] == 1 || pa[0][1] == 3))
		{
			if(Cube[id][7] == 0 && Cube[id][9] != 0)
			Cube[id][6] = Cube[id][9];
			else if(Cube[id][9] == 0 && Cube[id][7] != 0)
			Cube[id][6] = Cube[id][7];
			if(pa[0][1] == 1) Cube[id][9] = Cube[id][6];
			if(pa[0][1] == 3) Cube[id][7] = Cube[id][6];
		}
    	if(pa[0][1] == 0)
    	{
    		Cube[id][1] = crtp[0][1] - z;
    		Cube[id][0] = crtp[0][0] + z / 5;
    	}
    	else if(pa[0][1] == 1)
    	{
	    	if(pa[1][1] == 1)
	    	{
	    		Cube[id][9] = Cube[id][7];
	    		Cube[id][7] = 0;
	    	}
    		Cube[id][0] = crtp[0][0] - z;
    		Cube[id][1] = crtp[0][1] + z / 5;
    	}
    	else if(pa[0][1] == 2)
    	{
    		Cube[id][1] = crtp[0][1] - z;
    		Cube[id][0] = crtp[0][0] - 2 * z - z / 5;
    	}
    	else if(pa[0][1] == 3)
    	{
    		if(pa[1][1] == 3)
	    	{
	    		Cube[id][7] = Cube[id][9];
	    		Cube[id][9] = 0;
	    	}
    	    Cube[id][1] = crtp[0][1] - 2 * z - z / 5;
    		Cube[id][0] = crtp[0][0] - z;
    	}
	}}
}
void OpenDoor(int x,int y)
{
	if(anomap[y][x] >= 7 && anomap[y][x] <= 10 && f % (fps / 16) == 0){
	if(anomap[y][x] == 7)
	{
	    map[y + 2][x] = 0;
	}
	else if(anomap[y][x] == 10)
	{
	    map[y - 1][x] = 0;
	    map[y - 2][x] = 0;
	    map[y + 1][x] = 0;
	    map[y][x] = 0;
	}
	++anomap[y][x];}
}
void CloseDoor(int x,int y,int p)
{
	if(anomap[y][x] >= 8 && anomap[y][x] <= 11 && f % (fps / 16) == 0){
	if(anomap[y][x] == 11)
	{
	    map[y - 1][x] = p;
	    map[y - 2][x] = p;
	    map[y + 1][x] = p;
	    map[y][x] = p;
	}
	else if(anomap[y][x] == 8)
	{
	    map[y + 2][x] = p;
	}
	--anomap[y][x];}
}
void spread(int y,int x)
{
	map2[y][x] = 1;
	if(map2[y][x + 1] != 1 && map[y][x + 1] != 2 
	&& map[y][x + 1] != 1)
	spread(y,x + 1);
	if(map2[y + 1][x] != 1 && map[y + 1][x] != 2 
	&& map[y + 1][x] != 1)
	spread(y + 1,x);
	if(map2[y][x - 1] != 1 && map[y][x - 1] != 2
	&& map[y][x - 1] != 1)
	spread(y,x - 1);
	if(map2[y - 1][x] != 1 && map[y - 1][x] != 2 
	&& map[y - 1][x] != 1)
	spread(y - 1,x);
}
//INTRO FUNC:
void intro()
{
	SDL_Surface *srf = image("Demo/pngs/intro/Aperture.png",0);
	bool q = 0; int m = 1;
	SDL_Event e;
	int a = 0,t1,t2;
	SDL_Delay(3000);
	while(q == 0)
	{
		t1 = SDL_GetTicks();
		if(m == 1)
		a += 4;
		else if(m == 0)
		a -= 4;
		else if(m == 3)
		{
			a += 4;
			if(a >= 255)
			{
				a = 255;
				m = 4;
			}
			goto label;
		}
		else if(m == 4)
		{
			a -= 4;
			if(a <= 0) q = 1;
			goto label;
		}
		if(a >= 255)
		{
		    m = 0;
		    a = 255;
		}
		else if(a <= 50)
		m = 1;
		label:
		SDL_SetAlpha(srf,SDL_SRCALPHA,a);
		BlitSurface(srf,0,0,0,0,1281,721);
        SDL_Flip(scr);
        clrscr(2);
		while(SDL_PollEvent(&e))
        {
        	if(e.type == SDL_KEYDOWN)
            {
            	if(m == 1) m = 3;
                else if(m == 0) m = 4;
            }    
        }
        t2 = SDL_GetTicks() - t1;
        if(1000 / fps > t2)
        SDL_Delay(1000 / fps - t2);
	}
	clrscr(2);
	SDL_Flip(scr);
	SDL_FreeSurface(srf);
	srf = image("Demo/pngs/intro/HowToPlay.png",0);
	q = 0; a = 0; m = 0;
	SDL_Delay(3000);
	while(q == 0)
	{
		if(m == 0) 
		{
			a += 4;
			if(a >= 255)
			{
				a = 255;
				m = 2;
			}
		}
		else if(m == 1)
		{
			a -= 4;
			if(a <= 0) q = 1;
		}
		SDL_SetAlpha(srf,SDL_SRCALPHA,a);
		BlitSurface(srf,0,0,0,0,1281,721);
        SDL_Flip(scr);
        clrscr(2);
		while(SDL_PollEvent(&e))
		{
        	if(e.type == SDL_KEYDOWN)
            {
            	if(m == 2)
            	m = 1;
            }    
        }
	}
	clrscr(2);
	SDL_Flip(scr);
	SDL_Delay(3000);
	SDL_FreeSurface(srf);
}