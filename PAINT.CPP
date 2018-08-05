#include <iostream.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>

void init();
void doC(int c);
void clSc();
void clDisplay();
void normalizeOtherColor();
void saveWindowArea();
void saveDisplayArray();
void save ();
void load ();
void renderSaveLoadWindow(int stat);
void cleanRenderWindow();
void openSaveLoadWindow();
void closeSaveLoadWindow();
void reDrawWindowArea();
void showStat(int stat);
void statDeleter();
void funcFiller();
void fill (int xx,int yy,int p,int c);

int x = 43;
int y = 12;
int xp = x;
int yp = y;

int keyPr = 1;
int c = 0;

int far *sc = (int far*) 0xb8000000;
int displayArea[2000] = {0};
int displayAreaForLoad[2000] = {0};
int windowArea[800] = {0};
int windowPreRendered[800] = {0};
int color = 4;
int painting = 0;
int pastColor = 0;
int isDot = 1;

int xf = x;
int yf = y;
int isFillNeeded = 0;
int colorF = color;

//window properties
int isWindowOpen = 0;
int textboxLength = 0;
char textboxInput [13] = "f";
int statBackground[11] = {0};

void main() {

	init();

 // pressed key ascii -> int keyPr
	while(keyPr != 27) {

		if(!keyPr) {
			keyPr = getch();
			doC(keyPr);

		}else{
			doC(keyPr);
		}

		keyPr = getch();
	}
}

//function
void doC (int c) {


	//arrow keys
	if ((c==77||c==75||c==72||c==80) && !isWindowOpen) {

		if(!painting) {
			sc[yp*80+xp] = pastColor;
		}else{
			if(!isDot) {
				sc[yp*80+xp] = color*256 + 254;
			}else{
				sc[yp*80+xp] = color*16*256;
			}
		}

		if(c==77) {
			//right
			if(x<78 && (x!=8 || y!=23) && (x!=16 || y!=23)) x++;
		}else if(c==75) {
			//left
			if(x>8 && (x!=15 || y!=23) && (x!=23 || y!=23)) x--;
		}else if(c==72) {
			//up
			if(y>1) y--;
		}else if (c==80) {
			//down 10-15
			if(y<23 && !(x>8 && x<15 && y==22) && !(x>16 && x<23 && y==22)) y++;
		}

		pastColor = sc[y*80+x];
		sc[y*80+x] = (color+7*16+128)*256 + 254;

		xp = x;
		yp = y;
		

		if (isFillNeeded) {
			sc[yf*80+xf] = colorF*16*256;
			isFillNeeded = 0;
		}
	}


	//typing in textbox
	if(isWindowOpen) {
		if (c==56) {
			closeSaveLoadWindow();
			isWindowOpen = !isWindowOpen;
			save();
		}
		if (c==57) {
			closeSaveLoadWindow();
			isWindowOpen = !isWindowOpen;
			load();
		}
		//backspace
		if (c==8 && textboxLength > 0) {
			if(textboxLength==1) {
				textboxInput[0] = 'f';
			}else{
				textboxInput[textboxLength-1] = NULL;
			}
			textboxLength --;
			sc[3*80+(textboxLength+65)] = 1*16*256;
			
			if(textboxLength==0) {

				for (int p=65;p<77;p++) {
					sc[3*80+p] = 1*16*256;
				}

				sc[3*80+65] = (14+1*16)*256 +'F';
				sc[3*80+65+1] = (14+1*16)*256 +'i';
				sc[3*80+65+2] = (14+1*16)*256 +'l';
				sc[3*80+65+3] = (14+1*16)*256 +'e';
				sc[3*80+65+4] = (14+1*16)*256 +'n';
				sc[3*80+65+5] = (14+1*16)*256 +'a';
				sc[3*80+65+6] = (14+1*16)*256 +'m';
				sc[3*80+65+7] = (14+1*16)*256 +'e';
				sc[3*80+65+8] = (14+1*16)*256 +'.';
				sc[3*80+65+9] = (14+1*16)*256 +'.';
				sc[3*80+65+10] = (14+1*16)*256 +'.';
				textboxInput[0] = 'f';

			}
		}

		//only alphabets and numbers
			if((c>47 && c< 56) || (c>64 && c<91) || (c>96 && c<123)) {
				if(textboxLength < 12 && textboxLength > 0) {
					sc[3*80+(textboxLength+65)] = ((14+1*16)*256) + c;
					textboxLength++;
					textboxInput[textboxLength-1]= c;
					
				}else if(textboxLength==0) {
					for (int p=65;p<77;p++) {
						sc[3*80+p] = 1*16*256;
					}	
					sc[3*80+(textboxLength+65)] = ((14+1*16)*256) + c;
					textboxLength++;
					textboxInput[textboxLength-1]= c;
				}
			}
	}

	//39 -> open Save and Load Window
	if(c==39) {
		if(!isWindowOpen) {
			openSaveLoadWindow(); 
		}else{
			closeSaveLoadWindow();
		}
		isWindowOpen = !isWindowOpen;
	}

	//49-54 -> 1-6
	if ((c>48 && c<55) && !isWindowOpen) {
		normalizeOtherColor();
		switch(c) {
			case 49:
				color = 0;
				sc[y*80+x] = (color+7*16+128)*256 + 254;
				sc[2*80+2] = (0+ 7*16+128)*256 + 'E';

				break;
			case 50:
				color = 4;
				sc[y*80+x] = (color+7*16+128)*256 + 254;
				sc[4*80+2] = (4+ 7*16+128)*256 + 'R';
				break;
			case 51:
				color = 1;
				sc[y*80+x] = (color+7*16+128)*256 + 254;
				sc[6*80+2] = (1+ 7*16+128)*256 + 'B';
				break;
			case 52:
				color = 5;
				sc[y*80+x] = (color+7*16+128)*256 + 254;
				sc[8*80+2] = (5+ 7*16+128)*256 + 'P';
				break;
			case 53:
				color = 6;
				sc[y*80+x] = (color+7*16+128)*256 + 254;
				sc[10*80+2] = (6+ 7*16+128)*256 + 'O';
				break;
			case 54:
				color = 2;
				sc[y*80+x] = (color+7*16+128)*256 + 254;
				sc[12*80+2] = (2+ 7*16+128)*256 + 'G';
				break;
		}
	}

	if (c==113 && !isWindowOpen) {
		isDot = !isDot; //dot toggle (Q) *
		if(isDot) {
			sc[20*80+5] = 7*16*256;
		}else{
			sc[20*80+5] = 7*16*256 +254;
		}
	}

	// tab *
	if(c==9 && !isWindowOpen) {
		painting = !painting;
		if(!painting) {
			sc[17*80+5] = 7*16*256;
		}else{
			sc[17*80+5] = 7*16*256 +254;
		}
	}

	if(c==61 && !isWindowOpen) init(); // = *

	// f + color *
	if(c==102 && !isWindowOpen) {
		c = getch();

		//49-54 -> 1-6
		if(c>48 && c<55) {
			switch(c) {
				case 49:
					color = 0;
					break;
				case 50:
					color = 4;
					break;
				case 51:
					color = 1;
					break;
				case 52:
					color = 5;
					break;
				case 53:
					color = 6;
					break;
				case 54:
					color = 2;
					break;
			}

			if(color*16*256 != pastColor) {
				xf = x;
				yf = y;
				colorF = color;
				isFillNeeded = 1;

				fill(x,y,pastColor,color);
			}
		}
	}

}

//save 
int num = 0;

void save() {
	num = 0;
	FILE* savefile = fopen(textboxInput, "wb");
	for (int i=8;i<79;i++) {
		for (int j=1;j<24;j++) {
			if(i!=x || j!= y) {
				num = displayArea[j*80+i];
			}else{
				num = pastColor;
			}
			fprintf(savefile, "%d|", num);
		}
	}
	fclose(savefile);
	for (int a=0;a<textboxLength;a++) {
		if(a==0) {
			textboxInput[0] = 'f';
		}else{
			textboxInput[a] = NULL;
		}
	}
	showStat(0);
}

//load 
void load() {

	FILE* loadFile = fopen(textboxInput, "r");
	if(loadFile == NULL) {
		for (int b=0;b<textboxLength;b++) {
			if(b==0) {
				textboxInput[0] = 'f';
			}else{
				textboxInput[b] = NULL;
			}
		}
		showStat(2);
		funcFiller();
	}else{
		for (int i=8;i<79;i++) {
			for (int j=1;j<24;j++) {
				fscanf(loadFile, "%d|", &(displayAreaForLoad[j*80+i]));
			}
		}
		for (int k=8;k<79;k++) {
			for (int q=1;q<24;q++) {
				if(k!=x || q!=y) {
					sc[q*80+k] = displayAreaForLoad[q*80+k];
				}else{
					pastColor = displayAreaForLoad[q*80+k];
				}
			}
		}

		fclose(loadFile);
		for (int a=0;a<textboxLength;a++) {
			if(a==0) {
				textboxInput[0] = 'f';
			}else{
				textboxInput[a] = NULL;
			}
		}
		showStat(1);
	}

}



//fill
int pp,cp = 0;

void fill (int xx,int yy,int p,int c) {

	pp = p*16*256;
	cp = c*16*256;

	if (xx==xf && yy == yf) {

		if(xx<78) fill(xx+1,yy,p,c);

		if(xx>8) fill(xx-1,yy,p,c);

		if(yy>1) fill(xx,yy-1,p,c);

		if(yy<23) fill(xx,yy+1,p,c);

	}else if (sc[yy*80+xx] == pastColor) {
		sc[yy*80+xx] = cp;

		if(xx<78) fill(xx+1,yy,p,c);

		if(xx>8) fill(xx-1,yy,p,c);

		if(yy>1) fill(xx,yy-1,p,c);

		if(yy<23) fill(xx,yy+1,p,c);
	}
}

//status of file operation
void showStat(int stat) {
	for (int i=63;i<74;i++) {
		statBackground[i] = sc[23*80+i];
	}
	if (stat==0) {
		sc[23*80+63] = 7*256 +'S';
		sc[23*80+64] = 7*256 +'a';
		sc[23*80+65] = 7*256 +'v';
		sc[23*80+66] = 7*256 +'e';
		sc[23*80+67] = 7*256 +'d';
	}
	if (stat==1) {
		sc[23*80+63] = 7*256 +'L';
		sc[23*80+64] = 7*256 +'o';
		sc[23*80+65] = 7*256 +'a';
		sc[23*80+66] = 7*256 +'d';
		sc[23*80+67] = 7*256 +'e';
		sc[23*80+68] = 7*256 +'d';
	}
	if (stat==2) {
		sc[23*80+63] = 7*256 +'N';
		sc[23*80+64] = 7*256 +'o';
		sc[23*80+65] = 7*256 +'t';
		sc[23*80+66] = 7*256 +' ';
		sc[23*80+67] = 7*256 +'F';
		sc[23*80+68] = 7*256 +'o';
		sc[23*80+69] = 7*256 +'u';
		sc[23*80+70] = 7*256 +'n';
		sc[23*80+71] = 7*256 +'d';
		sc[23*80+72] = 7*256 +'e';
		sc[23*80+73] = 7*256 +'d';
	}
	statDeleter();
}

void statDeleter() {
	for (int i=63;i<74;i++) {
		delay(100);
		sc[23*80+i] = statBackground[i];
	}
}

//screen cleaner
void clSc() {
	for (int i=0;i<2000;i++) {
		sc[i] = 0;
	}
}

//painted display cleaner
void clDisplay() {
	for (int j=0;j<21;j++) {
		for (int i=8;i<80;i++) {
			sc[j*80+i] = 0;
		}
	}
}

void saveWindowArea() {
	for (int i=63;i<79;i++) {
		for (int j=1;j<10;j++) {
			windowArea[j*80+i] = sc[j*80+i];
		}
	}
}

void saveDisplayArray() {
	for (int i=8;i<79;i++) {
		for (int j=1;j<24;j++) {
			if(i!=x || j!=y) {
				displayArea[j*80+i] = sc[j*80+i];
			}else{
				displayArea[j*80+i] = pastColor;
			}

		}
	}
}

void openSaveLoadWindow() {
	saveDisplayArray();
	saveWindowArea();
	renderSaveLoadWindow(1);
}

int xr = 79;



void cleanRenderWindow() {
	for (int i=63;i<79;i++) {
			for (int j=1;j<10;j++) {
				sc[j*80+i] = 0;
			}
		}
}

void closeSaveLoadWindow() {
	renderSaveLoadWindow(0);
	reDrawWindowArea();
	textboxLength = 0;
	for(int j=1;j<10;j++) {
		sc[j*80+79] = 7*256 + 186;
	}
}

void reDrawWindowArea() {
	for (int i=63;i<79;i++) {
		for (int j=1;j<10;j++) {
			sc[j*80+i] = windowArea[j*80+i];
		}
	}
}

void normalizeOtherColor() {
			for (int j=2;j<13;j+=2) {
			switch(j) {
				case 2:
					sc[j*80+2] = (0+ 7*16)*256 + 'E';
					break;
				case 4:
					sc[j*80+2] = (4+ 7*16)*256 + 'R';
					break;

				case 6:
					sc[j*80+2] = (1+ 7*16)*256 + 'B';
					break;

				case 8:
					sc[j*80+2] = (5+ 7*16)*256 + 'P' ;
					break;

				case 10:
					sc[j*80+2] = (6+ 7*16)*256 + 'O';
					break;

				case 12:
					sc[j*80+2] = (2+ 7*16)*256 + 'G';
					break;
			}
		}
}




void renderSaveLoadWindow(int stat){
	if(stat) {
		xr = 79;
	}else{
		xr = 63;
	}

	if(xr==79) {
		while (xr > 62) {
			cleanRenderWindow();

			//background
			for (int i=xr;i<xr+17;i++) {
				for (int j=1;j<10;j++) {
					if(i<80)sc[j*80+i] = 7*16*256;
				}
			}

			//background border
			sc[1*80+xr] = 7*16*256 +218;
			if(xr+16<80) sc[1*80+xr+16] = 7*16*256 +191;
			sc[9*80+xr] = 7*16*256 +192;
			if(xr+16<80) sc[9*80+xr+16] = 7*16*256 +217;

			for (int d=xr+1;d<xr+16;d++) {
				if(d<80) sc[1*80+d] = 7*16*256 +196;
				if(d<80) sc[9*80+d] = 7*16*256 +196;
			}

			for (int s=2;s<10;s++) {
				sc[s*80+xr] = 7*16*256 +179;
				if(xr+16<80) sc[s*80+xr+16] = 7*16*256 +179;
			}

			//textbox stuff
			if(xr+1<80) sc[3*80+xr+1] = 1*16*256;
			if(xr+15<80) sc[3*80+xr+15] = 1*16*256;

			for (int g=xr+2;g<xr+15;g++) {
				if(g<80) sc[3*80+g] = 1*16*256;
			}

			if(xr+2<80) sc[3*80+xr+2] = (14+1*16)*256 +'F';
			if(xr+3<80) sc[3*80+xr+2+1] = (14+1*16)*256 +'i';
			if(xr+4<80) sc[3*80+xr+2+2] = (14+1*16)*256 +'l';
			if(xr+5<80) sc[3*80+xr+2+3] = (14+1*16)*256 +'e';
			if(xr+6<80) sc[3*80+xr+2+4] = (14+1*16)*256 +'n';
			if(xr+7<80) sc[3*80+xr+2+5] = (14+1*16)*256 +'a';
			if(xr+8<80) sc[3*80+xr+2+6] = (14+1*16)*256 +'m';
			if(xr+9<80) sc[3*80+xr+2+7] = (14+1*16)*256 +'e';
			if(xr+10<80) sc[3*80+xr+2+8] = (14+1*16)*256 +'.';
			if(xr+11<80) sc[3*80+xr+2+9] = (14+1*16)*256 +'.';
			if(xr+12<80) sc[3*80+xr+2+10] = (14+1*16)*256 +'.';

			//title
			if(xr+2<80) sc[6*80+xr+2] = 7*16*256 +'S';
			if(xr+3<80) sc[6*80+xr+2+1] = 7*16*256 +'a';
			if(xr+4<80) sc[6*80+xr+2+2] = 7*16*256 +'v';
			if(xr+5<80) sc[6*80+xr+2+3] = 7*16*256 +'e';
			if(xr+4<80) sc[7*80+xr+2] = 7*16*256 + 192;
			if(xr+3<80) sc[7*80+xr+2+1] = 7*16*256 + '(';
			if(xr+4<80) sc[7*80+xr+2+2] = 7*16*256 + '8'; //56
			if(xr+5<80) sc[7*80+xr+2+3] = 7*16*256 + ')';

			if(xr+7<80) sc[6*80+xr+7] = 7*16*256 +178;
			if(xr+8<80) sc[6*80+xr+7+1] = 7*16*256 +178;	
			if(xr+7<80) sc[7*80+xr+7] = 7*16*256 +178;
			if(xr+8<80) sc[7*80+xr+7+1] = 7*16*256 +178;

			if(xr+10<80) sc[6*80+xr+7+3] = 7*16*256 +'L';
			if(xr+11<80) sc[6*80+xr+7+4] = 7*16*256 +'o';
			if(xr+12<80) sc[6*80+xr+7+5] = 7*16*256 +'a';
			if(xr+13<80) sc[6*80+xr+7+6] = 7*16*256 +'d';
			if(xr+10<80) sc[7*80+xr+7+3] = 7*16*256 + 192;
			if(xr+11<80) sc[7*80+xr+7+4] = 7*16*256 + '(';
			if(xr+12<80) sc[7*80+xr+7+5] = 7*16*256 + '9'; //57
			if(xr+13<80) sc[7*80+xr+7+6] = 7*16*256 + ')';

			//moving
			xr--;
			delay(10);
		}
	}else{
		while (xr < 80) {
			cleanRenderWindow();

			//background
			for (int i=xr;i<xr+17;i++) {
				for (int j=1;j<10;j++) {
					if(i<80)sc[j*80+i] = 7*16*256;
				}
			}

			//background border
			sc[1*80+xr] = 7*16*256 +218;
			if(xr+16<80) sc[1*80+xr+16] = 7*16*256 +191;
			sc[9*80+xr] = 7*16*256 +192;
			if(xr+16<80) sc[9*80+xr+16] = 7*16*256 +217;

			for (int d=xr+1;d<xr+16;d++) {
				if(d<80) sc[1*80+d] = 7*16*256 +196;
				if(d<80) sc[9*80+d] = 7*16*256 +196;
			}

			for (int s=2;s<10;s++) {
				sc[s*80+xr] = 7*16*256 +179;
				if(xr+16<80) sc[s*80+xr+16] = 7*16*256 +179;
			}

			//textbox stuff
			if(xr+1<80) sc[3*80+xr+1] = 1*16*256;
			if(xr+15<80) sc[3*80+xr+15] = 1*16*256;

			for (int g=xr+2;g<xr+15;g++) {
				if(g<80) sc[3*80+g] = 1*16*256;
			}

			if(xr+2<80) sc[3*80+xr+2] = (14+1*16)*256 +'F';
			if(xr+3<80) sc[3*80+xr+2+1] = (14+1*16)*256 +'i';
			if(xr+4<80) sc[3*80+xr+2+2] = (14+1*16)*256 +'l';
			if(xr+5<80) sc[3*80+xr+2+3] = (14+1*16)*256 +'e';
			if(xr+6<80) sc[3*80+xr+2+4] = (14+1*16)*256 +'n';
			if(xr+7<80) sc[3*80+xr+2+5] = (14+1*16)*256 +'a';
			if(xr+8<80) sc[3*80+xr+2+6] = (14+1*16)*256 +'m';
			if(xr+9<80) sc[3*80+xr+2+7] = (14+1*16)*256 +'e';
			if(xr+10<80) sc[3*80+xr+2+8] = (14+1*16)*256 +'.';
			if(xr+11<80) sc[3*80+xr+2+9] = (14+1*16)*256 +'.';
			if(xr+12<80) sc[3*80+xr+2+10] = (14+1*16)*256 +'.';

			//title
			if(xr+2<80) sc[6*80+xr+2] = 7*16*256 +'S';
			if(xr+3<80) sc[6*80+xr+2+1] = 7*16*256 +'a';
			if(xr+4<80) sc[6*80+xr+2+2] = 7*16*256 +'v';
			if(xr+5<80) sc[6*80+xr+2+3] = 7*16*256 +'e';
			if(xr+4<80) sc[7*80+xr+2] = 7*16*256 + 192;
			if(xr+3<80) sc[7*80+xr+2+1] = 7*16*256 + '(';
			if(xr+4<80) sc[7*80+xr+2+2] = 7*16*256 + '8';
			if(xr+5<80) sc[7*80+xr+2+3] = 7*16*256 + ')';

			if(xr+7<80) sc[6*80+xr+7] = 7*16*256 +178;
			if(xr+8<80) sc[6*80+xr+7+1] = 7*16*256 +178;	
			if(xr+7<80) sc[7*80+xr+7] = 7*16*256 +178;
			if(xr+8<80) sc[7*80+xr+7+1] = 7*16*256 +178;

			if(xr+10<80) sc[6*80+xr+7+3] = 7*16*256 +'L';
			if(xr+11<80) sc[6*80+xr+7+4] = 7*16*256 +'o';
			if(xr+12<80) sc[6*80+xr+7+5] = 7*16*256 +'a';
			if(xr+13<80) sc[6*80+xr+7+6] = 7*16*256 +'d';
			if(xr+10<80) sc[7*80+xr+7+3] = 7*16*256 + 192;
			if(xr+11<80) sc[7*80+xr+7+4] = 7*16*256 + '(';
			if(xr+12<80) sc[7*80+xr+7+5] = 7*16*256 + '9';
			if(xr+13<80) sc[7*80+xr+7+6] = 7*16*256 + ')';

			//moving
			xr++;
			delay(10);
		}
	}
}
void funcFiller() {
		// fill layout
	sc[23*80+9] = 7*256 +201;
	sc[23*80+10] = 7*256 +'F';
	sc[23*80+11] = 7*256 +'I';
	sc[23*80+12] = 7*256 +'L';
	sc[23*80+13] = 7*256 +'L';
	sc[23*80+14] = 7*256 +187;

	sc[24*80+9] = 7*256 +205;
	sc[24*80+10] = 7*256 +'F';
	sc[24*80+11] = 7*256 +'+';
	sc[24*80+12] = 5*256 +'C';
	sc[24*80+13] = 5*256 +'L';
	sc[24*80+14] = 7*256 +205;

	//save layout

	sc[23*80+17] = 7*256 +201;
	sc[23*80+18] = 7*256 +'F';
	sc[23*80+19] = 7*256 +'I';
	sc[23*80+20] = 7*256 +'L';
	sc[23*80+21] = 7*256 +'E';
	sc[23*80+22] = 7*256 +187;

	sc[24*80+17] = 7*256 +205;
	sc[24*80+18] = 7*256 +'(';
	sc[24*80+19] = 5*256 +'"';
	sc[24*80+20] = 5*256 +'"';
	sc[24*80+21] = 7*256 +')';
	sc[24*80+22] = 7*256 +205;
}
//init
void init() {

	clSc();

	//declaring var
	x = 43;
	y = 12;
	xp = x;
	yp = y;
	painting = 0;
	pastColor = 0;
	color = 4;
	keyPr = 1;
	c = 0;
	xf = x;
	yf = y;
	isFillNeeded = 0;
	colorF = color;


	sc[y*80+x] = (color+7*16+128)*256 + 254;  //pointer base color


	sc[7] = 7*256+201;
	for (int j=8;j<79;j++) {
		sc[j] = 7*256 + 205;
	}
	sc[79] = 7*256+187;

	sc[24*80+7] = 7*256+200;
	for (int k=24*80+8;k<24*80+79;k++) {
		sc[k] = 7*256 + 205;
	}
	sc[24*80+79] = 7*256+188;

	for (int q=87;q<24*80;q+=80) {
		sc[q] = 7*256 + 186;
		sc[q+72] = 7*256 + 186;
	}

	for (int i = 0;i<15;i++) {
		if (i==0) {
			sc[i*80] = 7*256 +201;
			sc[i*80+1] = 7*256 +'C';
			sc[i*80+2] = 7*256 +'O';
			sc[i*80+3] = 7*256 +'L';
			sc[i*80+4] = 7*256 +'O';
			sc[i*80+5] = 7*256 +'R';
			sc[i*80+6] = 7*256 +187;
		}else{
			sc[i*80] = 7*256 +186;
			sc[i*80+1] = 7*16*256;
			sc[i*80+2] = 7*16*256;
			sc[i*80+3] = 7*16*256;
			sc[i*80+4] = 7*16*256;
			sc[i*80+5] = 7*16*256;
			sc[i*80+6] = 7*256 +186;
		}
		for (int j=2;j<24;j+=2) {
			switch(j) {
				case 2:
					sc[j*80] = (0+ 7*16)*256 + '1';
					sc[j*80+2] = (0+ 7*16)*256 + 'E';
					sc[j*80+4] = 0*16*256;
					break;
				case 4:

					sc[j*80] = (0+ 7*16)*256 + '2';
					sc[j*80+2] = (4+ 7*16)*256 + 'R';
					sc[j*80+4] = 4*16*256;
					break;

				case 6:
					sc[j*80] = (0+ 7*16)*256 + '3';
					sc[j*80+2] = (1+ 7*16)*256 + 'B';
					sc[j*80+4] = 1*16*256;
					break;

				case 8:
					sc[j*80] = (0+ 7*16)*256 + '4';
					sc[j*80+2] = (5+ 7*16)*256 + 'P' ;
					sc[j*80+4] = 5*16*256;
					break;

				case 10:
					sc[j*80] = (0+ 7*16)*256 + '5';
					sc[j*80+2] = (6+ 7*16)*256 + 'O';
					sc[j*80+4] = 6*16*256;
					break;

				case 12:
					sc[j*80] = (0+ 7*16)*256 + '6';
					sc[j*80+2] = (2+ 7*16)*256 + 'G';
					sc[j*80+4] = 2*16*256;
					break;

				case 14:

					sc[j*80] = 7*256 +200;
					for (int kk=1;kk<6;kk++) {
						sc[j*80+kk] = 7*256+240;
					}
					sc[j*80+6] = 7*256 +188;

					break;

				case 16:

					sc[j*80] = 7*256 +196;

					char ch[8] = " FUNCS";
					for (int k=1;k<7;k++) {
						if(k==6) {
							continue;
						}else{
							sc[j*80+k] = 7*256 + ch[k];
						}
					}
					sc[j*80+6] = 7*256 + 196;

					break;

				case 18:

					char chs[8] = " Flag";
					for (int jj=1;jj<7;jj++) {
						if(jj==6) {
							continue;
						}else{
							sc[(j-1)*80+jj] = 7*256 + chs[jj];
						}
					}
					sc[(j-1)*80+5] = 7*16*256;

					sc[j*80+1] = 7*256 + 192;
					char chss[3] = "TAB";
						for (int qj=0;qj<3;qj++) {
							sc[j*80 + qj+2] = 7*256 + chss[qj];
						}

					break;

				case 20:
					//21 > 22
					char chsd[8] = " Dot";
					for (int jkj=1;jkj<7;jkj++) {
						if(jkj==6) {
							continue;
						}else{
							sc[(j)*80+jkj] = 7*256 + chsd[jkj];
						}
					}
					sc[(j)*80+5] = 7*16*256;

					sc[(j+1)*80+1] = 7*256 +192;
					sc[(j+1)*80+2] = 7*256 +'Q';
					break;

				case 22:
					char resetC[8] = " Reset";
					for (int rc=1;rc<7;rc++) {
						if(rc==6) {
							continue;
						}else{
							sc[(j+1)*80+rc] = 7*256 + resetC[rc];
						}
					}

					sc[(j+2)*80+1] = 7*256 +192;
					sc[(j+2)*80+2] = 7*256 +'(';
					sc[(j+2)*80+3] = 7*256 +'=';
					sc[(j+2)*80+4] = 7*256 +')';

					break;
			}
		}
		sc[4*80+2] = (4+ 7*16+128)*256 + 'R';  //selected color
	}

	// fill layout
	sc[23*80+9] = 7*256 +201;
	sc[23*80+10] = 7*256 +'F';
	sc[23*80+11] = 7*256 +'I';
	sc[23*80+12] = 7*256 +'L';
	sc[23*80+13] = 7*256 +'L';
	sc[23*80+14] = 7*256 +187;

	sc[24*80+9] = 7*256 +205;
	sc[24*80+10] = 7*256 +'F';
	sc[24*80+11] = 7*256 +'+';
	sc[24*80+12] = 5*256 +'C';
	sc[24*80+13] = 5*256 +'L';
	sc[24*80+14] = 7*256 +205;

	//save layout

	sc[23*80+17] = 7*256 +201;
	sc[23*80+18] = 7*256 +'F';
	sc[23*80+19] = 7*256 +'I';
	sc[23*80+20] = 7*256 +'L';
	sc[23*80+21] = 7*256 +'E';
	sc[23*80+22] = 7*256 +187;

	sc[24*80+17] = 7*256 +205;
	sc[24*80+18] = 7*256 +'(';
	sc[24*80+19] = 5*256 +'"';
	sc[24*80+20] = 5*256 +'"';
	sc[24*80+21] = 7*256 +')';
	sc[24*80+22] = 7*256 +205;

}