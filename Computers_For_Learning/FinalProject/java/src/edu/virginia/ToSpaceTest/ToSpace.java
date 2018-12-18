package edu.virginia.ToSpaceTest;

import java.awt.Graphics;
import java.awt.Font;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;
import java.awt.event.*;
import java.awt.Point;
import java.awt.*;
import java.util.Collection;
import java.util.Locale;
import javax.swing.*;

import edu.virginia.engine.display.*;


/**
 * Example game that utilizes our engine. We can create a simple prototype game with just a couple lines of code
 * although, for now, it won't be a very fun game :)
 * */

// Names: Ni'Gere Epps & Zack Crenshaw
public class ToSpace extends Game implements MouseListener {


	/* Create a sprite object for our game. We'll use mario */
	AnimatedSprite rocket = new AnimatedSprite("rocket", "rocket.png", new Point(72,-20));
	Sprite openrocket = new Sprite("openrocket", "rocket_open.png", new Point(72,-20));
	Sprite liquidopenrocket = new Sprite("liquidor", "liquid_open_rocket.png", new Point(72,-20));
	Sprite background = new Sprite("background", "background.png", new Point(0, -3950));
	Sprite scientist1 = new Sprite("scientist1", "scientist1.png", new Point(0,600));
	Sprite settings = new Sprite("settings", "gear.png", new Point(1015,710));
	Sprite reset = new Sprite("reset", "reset.png", new Point(1105,620));
	Sprite help = new Sprite("help","help.png", new Point(1105,710));

	Sprite playing = new Sprite("playing", "soundon.png", new Point(1015,620));
	Sprite muted = new Sprite("muted","soundoff.png",new Point(1015,620));
	Sprite solidwhole = new Sprite("solidwhole", "1-1.png", new Point(500, 100));

	Sprite mute_menu = new Sprite("mute_menu","soundon.png", new Point(880,225));
	Sprite reset_menu = new Sprite("reset_menu","reset.png", new Point(880,265));
	Sprite settings_menu = new Sprite("settings_menu","gear.png", new Point(900,305));
	Sprite help_menu = new Sprite("help_menu","help.png", new Point(880,345));

	//ArrayList<Integer> checkpoints = init_checkpoints();


	private float highscore = 0;
	private int solidfuelselect = 0;
	private int liquidfuelselect = 0;
	private int solidfuelsplit = 0;
	private int liquidfuelsplit = 0;
	private float curheight = 0;
	private float maxheight = 0;
	private float lastLevel = 0;
	private int attempts = 0;
	private int level = 0;
	private Point mousePos = new Point(0,0);

	private SoundManager sounds = new SoundManager();
	private TextManager eng_texts = new TextManager();
	private TextManager esp_texts = new TextManager();


	private float gravity = .2f;
	private float DEFAULT_SPEED = 20;
	private float speed = DEFAULT_SPEED;

	private int gamestate = 0;
	private int lastGamestate = 0;
	private int textstate = 0;
	private int lastTextstate = textstate;
	private boolean allowContinue = true;


	private int launchstate = 0;

	private Font statsheading = new Font("Helvitica", Font.BOLD, 30);
	private Font stats = new Font(Font.SERIF, Font.ITALIC, 17);
	private Font textbox = new Font("Arial", Font.PLAIN, 24 );
	private Font pressspace = new Font("Times New Roman", Font.ITALIC, 22);
	private Font settings_font = new Font("Arial",Font.PLAIN, 18);

	private Color launchButtonRed = new Color(204, 0, 0);

	enum Language {ENGLISH, SPANISH;}
	private Language language = Language.ENGLISH;

	enum Music{HIP_HOP, EDM, POP, ROCK};
	private Music music = Music.HIP_HOP;
	private boolean text_to_speech = true;
	private boolean statsbox = true;
	private boolean readHeight = true;
	private int time = 0;
	private float startspeed = 0;




	private ToSpace() {

		super("To Space and Back", 1225, 850);
		getMainFrame().addMouseListener(this);

		rocket.setScaleX(1.55f);
		rocket.setScaleY(1.55f);

		openrocket.setScaleX(1.55f);
		openrocket.setScaleY(1.55f);

		liquidopenrocket.setScaleX(1.55f);
		liquidopenrocket.setScaleY(1.55f);

		solidwhole.setScaleX(.75f);
		solidwhole.setScaleY(.75f);

		settings.setScaleX(1.5f);
		settings.setScaleY(1.5f);

		help.setScale(1.5f);

		reset.setScaleX(1.5f);
		reset.setScaleY(1.5f);

		playing.setScaleX(1.5f);
		playing.setScaleY(1.5f);

		muted.setScaleX(1.5f);
		muted.setScaleY(1.5f);

		settings_menu.setScaleX(.5f);
		settings_menu.setScaleY(.5f);

		reset_menu.setScaleX(.5f);
		reset_menu.setScaleY(.5f);

		mute_menu.setScaleX(.5f);
		mute_menu.setScaleY(.5f);

		help_menu.setScaleX(.4f);
		help_menu.setScaleY(.4f);


		eng_texts.LoadTextfromFile("story_text.txt");
		esp_texts.LoadTextfromFile("story_text_esp.txt");


		sounds.LoadSFX(); //load sounds effects and music
		sounds.LoadTTS(LanguagetoString(Language.ENGLISH)); //load english text to speech
		sounds.LoadTTS(LanguagetoString(Language.SPANISH)); //load spanish text to speech

		sounds.PlayMusic(MusicToString(music));


/*
		Collection check = esp_texts.getValues();
		int i = 0;
		for (Object s : check) {
			System.out.println(""+ i++ +s);
		}
*/

	}


	public void mute() {
		//sound button
		//System.out.println("SOUND BUTTON");
		sounds.toggleMute();
		//System.out.println(sounds.getSoundon());

	}

	public void reset() {
		//reset button
		//System.out.println("RESET BUTTON");
		gamestate=0;
		curheight = 0;
		allowContinue = true;
		speed = DEFAULT_SPEED;
		//textstate = lastTextstate;
		sounds.StopSound("text"+textstate+LanguagetoString(language));
		if(textstate < 8) textstate = 0;
		else if (textstate < 17) textstate = 8;
		else if(textstate == 17) textstate = 13;
		else if (textstate <23) textstate = 19;
		else if (textstate <27) textstate = 23;
		else if(textstate == 27) textstate = 22;
		//attempts++;
		gamestate = 0;
		launchstate = 0;
		rocket.stopAnimation(0);
		sounds.StopSound("launched");
		if (text_to_speech)
			sounds.PlaySoundEffect("reset");
			Sleep(400);



	}

	public void settings() {
		//settings button
		//System.out.println("SETTINGS BUTTON");
		if (gamestate != 3)
			lastGamestate = gamestate;
		gamestate = 2;
		if (text_to_speech)
			sounds.PlaySoundEffect("settings");

	}

	public void help() {
		//help button
		//System.out.println("HELP BUTTON");
		if (gamestate != 2)
			lastGamestate = gamestate;
		gamestate = 3;
		if (text_to_speech)
			sounds.PlaySoundEffect("help");

	}

	public void launch() {
		if (launchstate == 1 && solidfuelselect != 0 && liquidfuelselect != 0) {
			sounds.PlaySoundEffect("launched");
			launchstate = 0;
			gamestate = 1;
			textstate++;
			attempts++;
			maxheight = 0;
			time = 0;
			readHeight = true;
			//startspeed = speed*5;
		}
	}

	public void add() {
		boolean change = false;
		if (textstate == 5 || textstate == 10) { //Intro adding fuel whole piece
				textstate++;
				solidfuelselect++;
				//liquidfuelselect++;
				//System.out.println(textstate);
			change = true;
			}


		if (textstate == 14 || textstate == 19) { //Clicking on number of pieces solid
				if (solidfuelselect < solidfuelsplit)
					solidfuelselect++;
			change = true;
			}



		if (textstate == 20) { //Clicking on liquid fuel pieces
				if (liquidfuelselect < 2)
					liquidfuelselect++;
			change = true;
		}


		if (textstate == 23) { //Clicking on number of solid pieces
				if (solidfuelselect < solidfuelsplit)
					solidfuelselect++;
			change = true;
		}

		if (textstate == 24 || textstate == 1) { //Clicking on liquid fuel pieces
				if (liquidfuelselect < liquidfuelsplit)
					liquidfuelselect++;
			change = true;
		}
		if (change && text_to_speech) {
			sounds.StopSound("text"+(textstate-1)+LanguagetoString(language));
			sounds.StopSound("text"+textstate+LanguagetoString(language));
			sounds.PlaySoundEffect("added");
			Sleep(1200);
		}

	}

	public String MusicToString(Music m) {
		switch (m){
			case HIP_HOP: return "HIP_HOP";
			case EDM: return "EDM";
			case POP: return "POP";
			default: return "ROCK";
		}
	}

	public String LanguagetoString(Language lang){
		switch (lang){
			case SPANISH: return "_SPANISH";
			default: return "_ENGLISH";
		}
	}

	public void switchMusic(Music m) {
		if (m != music)
			sounds.StopSound(MusicToString(music));
			music = m;
			sounds.PlayMusic(MusicToString(music));
	}

	@Override
	public void mouseClicked(MouseEvent e) {
		mousePos = new Point(e.getX(),e.getY());
		//mousePos = new Point(SwingUtilities.convertPointFromScreen(MouseInfo.getPointerInfo().getLocation(), component));
		//System.out.println(mousePos);

		//System.out.println(mousePos.getX() + " , " + mousePos.getY());

		if (gamestate == 2 ){ //settings
			if (mousePos.getX() > 135 && mousePos.getX() < 365 &&
					mousePos.y > 640 && mousePos.y < 700) {
				gamestate = lastGamestate;
				//System.out.println("BACK TO GAME");
			}

			if (mousePos.getX() > 310 && mousePos.getX() < 330 &&
					mousePos.getY() > 185 && mousePos.getY() < 200) {
				text_to_speech = true; //turn on tts
			}

			if (mousePos.getX() > 400 && mousePos.getX() < 420 &&
					mousePos.getY() > 185 && mousePos.getY() < 200) {
				text_to_speech = false; //turn off tts
			}

			if (mousePos.getX() > 310 && mousePos.getX() < 330 &&
					mousePos.getY() > 265 && mousePos.getY() < 285) {
				if (language != Language.ENGLISH) { //set language to english
					sounds.StopSound("text" + textstate + LanguagetoString(language));
					//System.out.println("text"+textstate+LanguagetoString(language));
					language = Language.ENGLISH;
				}
			}

			if (mousePos.getX() > 400 && mousePos.getX() < 420 &&
					mousePos.getY() > 265 && mousePos.getY() < 285) {
				if (language != Language.SPANISH) { //set language to english
					sounds.StopSound("text" + textstate + LanguagetoString(language));
					//System.out.println("text"+textstate+LanguagetoString(language));
					language = Language.SPANISH;
				}
			}


			if (mousePos.getX() > 310 && mousePos.getX() < 330 &&
					mousePos.getY() > 365 && mousePos.getY() < 385) {
				switchMusic(Music.HIP_HOP);
			}

			if (mousePos.getX() > 400 && mousePos.getX() < 420 &&
					mousePos.getY() > 365 && mousePos.getY() < 385) {
				switchMusic(Music.EDM);
			}

			if (mousePos.getX() > 490 && mousePos.getX() < 510 &&
					mousePos.getY() > 365 && mousePos.getY() < 385) {
				switchMusic(Music.POP);
			}

			if (mousePos.getX() > 580 && mousePos.getX() < 600 &&
					mousePos.getY() > 365 && mousePos.getY() < 385) {
				switchMusic(Music.ROCK);
			}

			if (mousePos.getX() > 310 && mousePos.getX() < 330 &&
					mousePos.getY() > 465 && mousePos.getY() < 495) {
			    statsbox = true;
			}

			if (mousePos.getX() > 400 && mousePos.getX() < 420 &&
					mousePos.getY() > 465 && mousePos.getY() < 495) {
				statsbox = false;
			}







		} if (gamestate == 3){ //help
			if (mousePos.getX() > 135 && mousePos.getX() < 365 &&
					mousePos.y > 640 && mousePos.y < 700) {
				gamestate = lastGamestate;
				//System.out.println("BACK TO GAME");
			}


		} else {
			if (mousePos.getX() > 1015 && mousePos.getX() < 1105 &&
					mousePos.getY() > 620 && mousePos.getY() < 700) {
				mute();
			}

				if (mousePos.getX() > 1105 && mousePos.getX() < 1195 &&
						mousePos.getY() > 620 && mousePos.getY() < 700) {
					reset();
				}

			if (mousePos.getX() > 1015 && mousePos.getX() < 1105 &&
					mousePos.getY() > 710 && mousePos.getY() < 790) {
				settings();
			}

			if (mousePos.getX() > 1105 && mousePos.getX() < 1195 &&
					mousePos.getY() > 710 && mousePos.getY() < 790) {
				help();
			}


			if (mousePos.getX() > 500 && mousePos.getX() < 875 &&
					mousePos.getY() > 100 && mousePos.getY() < 475) {
				if (textstate == 5 || textstate == 10) { //Intro adding fuel whole piece
					textstate++;
					solidfuelselect++;
					//liquidfuelselect++;
					//System.out.println(textstate);
				}
			}

			if (textstate == 14 || textstate == 19) { //Clicking on number of pieces solid
				if (mousePos.getX() > 500 && mousePos.getX() < 875 &&
						mousePos.getY() > 100 && mousePos.getY() < 475) {
					if (solidfuelselect < solidfuelsplit)
						solidfuelselect++;
				}
			}


			if (textstate == 20) { //Clicking on liquid fuel pieces
				if (mousePos.getX() > 500 && mousePos.getX() < 875 &&
						mousePos.getY() > 100 && mousePos.getY() < 640) {
					if (liquidfuelselect < 2)
						liquidfuelselect++;
				}
			}

			if (textstate == 23) { //Clicking on number of solid pieces
				if (mousePos.getX() > 500 && mousePos.getX() < 875 &&
						mousePos.getY() > 100 && mousePos.getY() < 475) {
					if (solidfuelselect < solidfuelsplit)
						solidfuelselect++;
				}
			}

			if (textstate == 24 || textstate == 1) { //Clicking on liquid fuel pieces
				if (mousePos.getX() > 500 && mousePos.getX() < 875 &&
						mousePos.getY() > 20 && mousePos.getY() < 640) {
					if (liquidfuelselect < liquidfuelsplit)
						liquidfuelselect++;
				}
			}

			//Launch Button
			if (mousePos.getX() > 1030 && mousePos.getX() < 1175 &&
						mousePos.getY() > 425 && mousePos.getY() < 570) {
					launch();
			}
		}


	}



	@Override
	public void mouseEntered(MouseEvent e) { }

	@Override
	public void mouseExited(MouseEvent e) { }

	@Override
	public void mousePressed(MouseEvent e) {
	}

	@Override
	public void mouseReleased(MouseEvent e) {
	}

	/**
	 * Engine will automatically call this update method once per frame and pass to us
	 * the set of keys (as strings) that are currently being pressed down
	 * */
	@Override
	public void update(ArrayList<Integer> pressedKeys){
		super.update(pressedKeys);

		if (gamestate != 3) {
			//System.out.println("here!");
			sounds.StopSound("help");
		}

		if (gamestate != 2) {
			sounds.StopSound("settings");
		}

		if (launchstate == 1) {
			startspeed = speed*5;
		}


		if (lastLevel != level) {
			attempts = 0;
		}


		if (pressedKeys.contains(KeyEvent.VK_ESCAPE)){
			gamestate = lastGamestate;
		}


		if (pressedKeys.contains(KeyEvent.VK_K)){
			sounds.StopSound(MusicToString(music));
		}

		if (pressedKeys.contains(KeyEvent.VK_B)) {
			statsbox = !statsbox;
			Sleep(400);
		}
		if (pressedKeys.contains(KeyEvent.VK_T)) {
			//System.out.println("TTS");
				text_to_speech = !text_to_speech;
				sounds.StopSound("text"+textstate+LanguagetoString(language));
				Sleep(400);

		}

			if (pressedKeys.contains(KeyEvent.VK_E)) {
				//System.out.println(language);
				//System.out.println("switch");
				if (language != Language.ENGLISH) {
					sounds.StopSound("text" + textstate + LanguagetoString(language));
					//System.out.println("text"+textstate+LanguagetoString(language));
					language = Language.ENGLISH;
					Sleep(400);
				}
			}

			if (pressedKeys.contains(KeyEvent.VK_N)) {
				//System.out.println(language);

				if (language != Language.SPANISH) {
					sounds.StopSound("text" + textstate + LanguagetoString(language));
					//System.out.println("text"+textstate+LanguagetoString(language));
					language = Language.SPANISH;
					Sleep(400);
				}
			}

			if (pressedKeys.contains(KeyEvent.VK_Z)) {
				switchMusic(Music.HIP_HOP);
				Sleep(400);

			}

			if (pressedKeys.contains(KeyEvent.VK_X)) {
				switchMusic(Music.EDM);
				Sleep(400);

			}

			if (pressedKeys.contains(KeyEvent.VK_C)) {
				switchMusic(Music.POP);
				Sleep(400);

			}

			if (pressedKeys.contains(KeyEvent.VK_V)) {
				switchMusic(Music.ROCK);
				Sleep(400);

			}




			if (pressedKeys.contains(KeyEvent.VK_M)) {
				mute();
				Sleep(400);
			}

			if (pressedKeys.contains(KeyEvent.VK_S)) {
				settings();
				Sleep(400);
			}

			if (pressedKeys.contains(KeyEvent.VK_H)) {
				help();
				Sleep(400);
			}

			if (pressedKeys.contains(KeyEvent.VK_R)) {
				reset();
				Sleep(400);
			}

			if (pressedKeys.contains(KeyEvent.VK_L)) {
				launch();
				Sleep(400);

			}

			if (pressedKeys.contains(KeyEvent.VK_A)) {
				add();
				Sleep(400);
			}



		/*
		if (checkpoints.contains(textstate)) {
			allowContinue = false;
			lastTextstate = textstate;
		}
		*/

			if (lastLevel != level) {
				maxheight = 0;
				lastLevel = level;
			}

			if (allowContinue) {

				if (pressedKeys.contains(KeyEvent.VK_SPACE)) {
					textstate++;
					//System.out.println("Text State: " + textstate);

					Sleep(400);

				}

				if (pressedKeys.contains(KeyEvent.VK_BACK_SPACE)) {
					if (textstate > 0) textstate--;
					Sleep(400);
				}
			}

			if (pressedKeys.contains(KeyEvent.VK_2))
				if (textstate == 9)
					textstate++;

			if (textstate == 14 || textstate == 23) { //Enter number to split into
				if (solidfuelsplit == 0) {
					if (pressedKeys.contains(KeyEvent.VK_2)) {
						solidfuelsplit = 2;
						if (text_to_speech) {
							sounds.StopSound("text"+textstate+LanguagetoString(language));
							sounds.PlaySoundEffect("split2");
							Sleep(2000);
						}
					}
					if (pressedKeys.contains(KeyEvent.VK_3)) {
						solidfuelsplit = 3;
						if (text_to_speech) {
							sounds.StopSound("text" + textstate + LanguagetoString(language));
							sounds.PlaySoundEffect("split3");
							Sleep(2000);

						}
					}
					if (pressedKeys.contains(KeyEvent.VK_4)) {
						solidfuelsplit = 4;
						if (text_to_speech) {
							sounds.StopSound("text"+textstate+LanguagetoString(language));
							sounds.PlaySoundEffect("split4");
							Sleep(2000);

						}
					}
					if (pressedKeys.contains(KeyEvent.VK_5)) {
						solidfuelsplit = 5;
						if (text_to_speech) {
							sounds.StopSound("text"+textstate+LanguagetoString(language));
							sounds.PlaySoundEffect("split5");
							Sleep(2000);

						}
					}
					if (pressedKeys.contains(KeyEvent.VK_6)) {
						solidfuelsplit = 6;
						if (text_to_speech) {
							sounds.StopSound("text"+textstate+LanguagetoString(language));
							sounds.PlaySoundEffect("split6");
							Sleep(2000);

						}
					}
					if (pressedKeys.contains(KeyEvent.VK_7)) {
						solidfuelsplit = 7;
						if (text_to_speech) {
							sounds.StopSound("text"+textstate+LanguagetoString(language));
							sounds.PlaySoundEffect("split7");
							Sleep(2000);

						}
					}
					if (pressedKeys.contains(KeyEvent.VK_8)) {
						solidfuelsplit = 8;
						if (text_to_speech) {
							sounds.StopSound("text"+textstate+LanguagetoString(language));
							sounds.PlaySoundEffect("split8");
							Sleep(2000);

						}
					}
				}
			}

			if (textstate == 24) { //Enter number to split into
				if (liquidfuelsplit == 0) {
					if (pressedKeys.contains(KeyEvent.VK_2)) {
						liquidfuelsplit = 2;
						if (text_to_speech) {
							sounds.StopSound("text"+textstate+LanguagetoString(language));
							sounds.PlaySoundEffect("split2");
							Sleep(2000);

						}
					}
					if (pressedKeys.contains(KeyEvent.VK_3)) {
						liquidfuelsplit = 3;
						if (text_to_speech) {
							sounds.StopSound("text"+textstate+LanguagetoString(language));
							sounds.PlaySoundEffect("split3");
							Sleep(2000);

						}
					}
					if (pressedKeys.contains(KeyEvent.VK_4)) {
						liquidfuelsplit = 4;
						if (text_to_speech) {
							sounds.StopSound("text"+textstate+LanguagetoString(language));
							sounds.PlaySoundEffect("split4");
							Sleep(2000);

						}
					}
					if (pressedKeys.contains(KeyEvent.VK_5)) {
						liquidfuelsplit = 5;
						if (text_to_speech) {
							sounds.StopSound("text"+textstate+LanguagetoString(language));
							sounds.PlaySoundEffect("split5");
							Sleep(2000);

						}
					}
					if (pressedKeys.contains(KeyEvent.VK_6)) {
						liquidfuelsplit = 6;
						if (text_to_speech) {
							sounds.StopSound("text"+textstate+LanguagetoString(language));
							sounds.PlaySoundEffect("split6");
							Sleep(2000);

						}
					}
					if (pressedKeys.contains(KeyEvent.VK_7)) {
						liquidfuelsplit = 7;
						if (text_to_speech) {
							sounds.StopSound("text" + textstate + LanguagetoString(language));
							sounds.PlaySoundEffect("split7");
							Sleep(2000);

						}
					}
					if (pressedKeys.contains(KeyEvent.VK_8)) {
						liquidfuelsplit = 8;
						if (text_to_speech) {
							sounds.StopSound("text" + textstate + LanguagetoString(language));
							sounds.PlaySoundEffect("split8");
							Sleep(2000);

						}
					}
				}
			}
/*
		if (checkpoints.contains(textstate)) {
			lastTextstate = textstate;
		}


*/

			if (gamestate == 1) {
				//readHeight = true;
				time++;

				curheight = 3950 + (float) background.getPosition().getY();
				//System.out.println(background.getY());


				if (maxheight < curheight) {
					maxheight = curheight;
				}

				if (highscore < maxheight) {
					highscore = maxheight;
				}
				System.out.println("time: "+ time+ " , speed: "+speed+ " , expected max: "+ startspeed);
				System.out.println(maxheight);
				//if (time == startspeed && text_to_speech) {
				if (readHeight && (maxheight > curheight) && text_to_speech) {
					readHeight = false;
					//System.out.println("text" + textstate + LanguagetoString(language));
					String mh = new String(String.valueOf(maxheight));
					sounds.ReadHeight(mh,"text"+(textstate)+LanguagetoString(language));
					//Sleep(mh.length()*500);
				}


			}

		}






	@Override
	public void draw(Graphics g) {
		super.draw(g);


		//System.out.println(speed);

		if (background != null) background.draw(g);

		if (gamestate >= 2) { //help or settings

			sounds.StopSound("text"+textstate+LanguagetoString(language));

			g.setColor(Color.black);
			g.fillRoundRect(90,90,1045,670,50,50);//border main

			g.setColor(Color.darkGray);
			g.fillRoundRect(100,100,1025,650,50,50);//main textbox

			g.setColor(Color.black);
			g.fillRoundRect(715,165,390,550,50,50); //border key bindings

			g.setColor(Color.darkGray);
			g.fillRoundRect(720,170,380,540,50,50);//key bindings box

			g.setColor(Color.white);//lettering
			g.setFont(statsheading);

			if (language == Language.SPANISH) {
				g.drawString("Teclas", 800, 200);
			} else {
				g.drawString("Key Bindings", 800, 200);


			}

			g.setColor(launchButtonRed);
			g.fillOval(860,418,45,45);
			g.setColor(Color.white);

			g.setFont(settings_font);

			if (language == Language.SPANISH) {
				g.drawString("Silencio:\t\t M o",730,250);
				g.drawString("Reiniciar:\t\t R o",730,290);
				g.drawString("Configuración:\t\t S o",730,330);
				g.drawString("Ayuda:\t\t H o",730,370);
				g.drawString("Añadir combustible:\t\t A o presiónalo",730,410);
				g.drawString("Lanzar:\t\t L o",730,450);
				g.drawString("Texto a habla:\t\t T (alternar)",730,490);
				g.drawString("Idioma:\t\t E para ENGLISH, N para ESPAÑOL",730,530);
				g.drawString("Música:\t\t Z: Hip-Hop, X: Electrónica ",730,570);
				g.drawString("C: Pop, V: Rock", 795, 590);
				g.drawString("Silenciar música:\t\tK", 795, 610);
				g.drawString("Esconder estadísticas\t\t: B",730, 650);
				g.drawString("Volver a juego:\t\t ESC",730,690);

				g.setFont(new Font("Helvetica",Font.BOLD,8));
				g.drawString("LANZAR",865,443);

			} else {
				g.drawString("Mute:\t\t M or",730,250);
				g.drawString("Reset:\t\t R or",730,290);
				g.drawString("Setings:\t\t S or",730,330);
				g.drawString("Help:\t\t H or",730,370);
				g.drawString("Add fuel:\t\t A or click fuel",730,410);
				g.drawString("Launch:\t\t L or",730,450);
				g.drawString("Text to Speech:\t\t T (toggles)",730,490);
				g.drawString("Language:\t\t E for ENGLISH, N for ESPAÑOL",730,530);
				g.drawString("Music:\t\t Z: Hip-Hop, X: Electronic ",730,570);
				g.drawString("C: Pop, V: Rock", 795, 590);
				g.drawString("Mute music\t\t: K", 795, 610);
				g.drawString("Hide stats box\t\t: B",730, 650);
				g.drawString("Back to Game:\t\t ESC",730,690);

				g.setFont(new Font("Helvetica",Font.BOLD,8));
				g.drawString("LAUNCH",865,443);


			}

			mute_menu.draw(g);
			settings_menu.draw(g);
			help_menu.draw(g);
			reset_menu.draw(g);

			g.setFont(statsheading);

			if (gamestate == 2) { //settings

				g.setColor(launchButtonRed);

				if (language == Language.SPANISH) {
					g.fillRoundRect(125, 600, 300,80,50,50);

					g.setColor(Color.white);

					g.drawString("CONFIGURACIÓN", 560, 140);
					g.drawString("VOLVER AL JUEGO", 130, 650);

					g.setFont(textbox);
					g.drawString("Texto a Habla", 125, 180);
					g.drawString("SÍ", 310, 210);
					g.drawString("NO", 400, 210);

					g.drawString("Idioma", 125, 270);
					g.drawString("English", 310, 310);
					g.drawString("Español", 400,310);

					g.drawString("Música",125,370);
					g.drawString("Hip-Hop",310,410);
					g.drawString("EDM",400,410);
					g.drawString("Pop",490,410);
					g.drawString("Rock",580,410);

					g.drawString("Estadísticas",125,470);
					g.drawString("SÍ", 310, 500);
					g.drawString("NO", 400, 500);



				} else {

					g.fillRoundRect(125, 600, 250,80,50,50);

					g.setColor(Color.white);

					g.drawString("SETTINGS", 560, 140);
					g.drawString("BACK TO GAME", 130, 650);

					g.setFont(textbox);
					g.drawString("Text to Speech", 125, 180);
					g.drawString("ON", 310, 210);
					g.drawString("OFF", 400, 210);

					g.drawString("Language", 125, 270);
					g.drawString("English", 310, 310);
					g.drawString("Español", 400,310);

					g.drawString("Music",125,370);
					g.drawString("Hip-Hop",310,410);
					g.drawString("EDM",400,410);
					g.drawString("Pop",490,410);
					g.drawString("Rock",580,410);

					g.drawString("Stat Box",125,470);
					g.drawString("ON", 310, 500);
					g.drawString("OFF", 400, 500);




				}
					if (text_to_speech)
						g.setColor(launchButtonRed);
					g.fillOval(310, 160, 20, 20);//t2s on
					g.setColor(Color.white);

					if (!text_to_speech)
						g.setColor(launchButtonRed);
					g.fillOval(400, 160, 20, 20);//t2s off
					g.setColor(Color.white);

					if (language == Language.SPANISH)
						g.setColor(launchButtonRed);
					g.fillOval(400,250,20,20); //spanish
					g.setColor(Color.white);

					if (language == Language.ENGLISH)
						g.setColor(launchButtonRed);
					g.fillOval(310,250,20,20); //english
					g.setColor(Color.white);

					if (music == Music.HIP_HOP)
						g.setColor(launchButtonRed);
					g.fillOval(310,350,20,20); //Hip-hop
					g.setColor(Color.white);

					if (music == Music.EDM)
						g.setColor(launchButtonRed);
					g.fillOval(400,350,20,20); //EDM
					g.setColor(Color.white);

					if (music == Music.POP)
						g.setColor(launchButtonRed);
					g.fillOval(490,350,20,20); //Pop
					g.setColor(Color.white);

					if (music == Music.ROCK)
						g.setColor(launchButtonRed);
					g.fillOval(580,350,20,20); //Rock
					g.setColor(Color.white);

					if (statsbox) {
						g.setColor(launchButtonRed);
					}
					g.fillOval(310,450,20,20);//stat on
					g.setColor(Color.white);

					if (!statsbox) {
						g.setColor(launchButtonRed);
					}
					g.fillOval(400,450,20,20);//stat off
					g.setColor(Color.white);


			} else if (gamestate == 3) { //help

				if (language == Language.SPANISH) {
					g.drawString("AYUDA", 560, 140);
				} else {
					g.drawString("HELP", 560, 140);
				}

				g.setFont(settings_font);

				if (language == Language.SPANISH) {
					g.drawString("Sigue las instrucciones de Dra. Stevens", 125, 175);
					g.drawString("Si hay combustible en la pantalla" +
							"presiona encima de él ", 125, 225);
					g.drawString("o presiona A para añadirlo en el cohete.",125,245);
					g.drawString("Cuando aparezca el botón de lanzamiento" +
									"presiónalo o", 125, 275);
					g.drawString("presiona L para lanzar el cohete",125,295);
					g.drawString("Sigue probando cantidades diferentes de combustible " +
							"para llegar", 125, 325);
					g.drawString("a la luna!", 125, 345);

					g.drawString("Presiona el símbolo de altavoz para alternar el silencio" , 125, 375);
					g.drawString("Presiona la flecha hacia atrás para reiniciar un nivel", 125, 425);
					g.drawString("Presiona el engranje para abrir la configuración", 125, 475);
					g.drawString("Presiona el signo de interrogación para ayuda", 125, 525);

				} else {
					g.drawString("Keep following Dr. Stevens' instructions", 125, 175);
					g.drawString("If there is fuel on the screen" +
							"click it or press A to add it to the rocket", 125, 225);
					g.drawString("When the launch button appears, hit it or press L to launch the rocket", 125, 275);
					g.drawString("Keep experimenting with the amounts of fuels " +
							"to get to the moon!", 125, 325);
					g.drawString("Press the speaker symbol to toggle mute", 125, 375);
					g.drawString("Press the backwards bendy arrow to restart a level", 125, 425);
					g.drawString("Press the gear button to open the settings", 125, 475);
					g.drawString("Press the question mark button to get help", 125, 525);
				}

				g.setFont(statsheading);
				g.setColor(launchButtonRed);

				if (language == Language.SPANISH) {
					g.fillRoundRect(125, 600, 300,80,50,50);
				} else {
					g.fillRoundRect(125, 600, 250,80,50,50);
				}


				g.setColor(Color.white);
				if (language == Language.SPANISH) {
					g.drawString("VOLVER AL JUEGO", 130, 650);
				} else {
					g.drawString("BACK TO GAME", 130, 650);
				}





			} if (gamestate > 3) { //not a valid gamestate
				g.drawString("ERROR: press ESC to return to game.", 500, 500);
			}



		} else {

			g.setColor(Color.black);
			g.fillRoundRect(0, 600, 200, 200, 50, 50); //dr
			g.fillRoundRect(250, 600, 700, 200, 50, 50); //text
			g.fillRoundRect(1000, 600, 200, 200, 50, 50); //settings


			g.setColor(Color.darkGray);
			g.fillRoundRect(5, 605, 190, 190, 50, 50);
			g.fillRoundRect(255, 605, 690, 190, 50, 50);
			g.fillRoundRect(1005, 605, 190, 190, 50, 50);



			if (launchstate == 1) {
				g.setColor(Color.black);
				g.fillOval(1015, 410, 175, 175);

				g.setColor(launchButtonRed);
				g.fillOval(1025, 420, 155, 155);
				//
			}


			if (statsbox) {

				g.setColor(Color.black);
				g.fillRoundRect(1000, 0, 200, 400, 50, 50); //stats

				g.setColor(Color.darkGray);
				g.fillRoundRect(1005, 5, 190, 390, 50, 50);

				g.setColor(Color.white);

				g.setFont(statsheading);
				g.drawString("STATS", 1050, 30);


				if (launchstate == 1) {
					if (language == Language.SPANISH) {
						g.drawString("LANZAR", 1040, 510);
					} else {
						g.drawString("LAUNCH", 1040, 510);
					}
				}

				g.setFont(stats);

				if (language == Language.SPANISH) {
					g.drawString("Nivel: " + this.level, 1007, 60);
					g.drawString("Puntuación alta: " + this.highscore, 1007, 110);
					g.drawString("Combustible sólido: " + this.solidfuelselect + " / " + this.solidfuelsplit, 1007, 160);
					g.drawString("Combustbile líquido: " + this.liquidfuelselect + " / " + this.liquidfuelsplit, 1007, 210);
					g.drawString("Altura actual: " + this.curheight, 1007, 260);
					g.drawString("Altura máxima: " + this.maxheight, 1008, 310);
					g.drawString("Intentos: " + this.attempts, 1008, 360);
				} else {
					g.drawString("Level: " + this.level, 1007, 60);
					g.drawString("High Score: " + this.highscore, 1007, 110);
					g.drawString("Solid Fuel Used: " + this.solidfuelselect + " / " + this.solidfuelsplit, 1007, 160);
					g.drawString("Liquid Fuel Used: " + this.liquidfuelselect + " / " + this.liquidfuelsplit, 1007, 210);
					g.drawString("Current Height: " + this.curheight, 1007, 260);
					g.drawString("Max Height: " + this.maxheight, 1008, 310);
					g.drawString("Attempts: " + this.attempts, 1008, 360);
				}
			}

			g.setFont(textbox);
			g.setColor(Color.white);



			switch (language) {
				case SPANISH :
					printText(esp_texts.GetText(textstate), g, 270, 625, textbox.getSize() + 5);

					break;
				default : printText(eng_texts.GetText(textstate), g, 270, 625, textbox.getSize() + 5);

			}
			if (text_to_speech)
				switch (textstate) {
					case 13: break;
					case 15: break;
					case 22: break;
					case 25: break;
					default:sounds.PlayTTS(textstate, LanguagetoString(language));
				}






			/* Same, just check for null in case a frame gets thrown in before things are initialized */
			if (settings != null) settings.draw(g);
			if (help != null) help.draw(g);
			if (reset != null) reset.draw(g);
			if (scientist1 != null) scientist1.draw(g);
			if (playing != null && sounds.getSoundon()) playing.draw(g);
			if (muted != null && !sounds.getSoundon()) muted.draw(g);
			if (rocket != null && rocket.getVisible() == 1) rocket.draw(g);


			if (gamestate == 0) { //Static Everything
				allowContinue = true;
				background.setPosition(new Point(0, -3950));
				g.setColor(Color.white);
				g.setFont(pressspace);
				if (textstate < 5 || textstate == 8 || textstate == 18)
					switch (language) {
						case SPANISH :
							g.drawString("Presiona SPACEBAR para continuar ", 600, 790);
							break;
							default :
								g.drawString("Press SPACEBAR to Continue", 670, 790);
					}

			}

//----------------------------------------------------------------------------------------------------------------------
//Text States
			if(textstate < 13) level = 1;
			if(textstate > 13 && textstate < 19) level = 2;
			if(textstate > 19 && textstate < 22) level = 3;
			if(textstate > 22) level = 4;


			if( (textstate > 1 && textstate < 6) || (textstate > 7 && textstate < 10) ) { //Intro to Ignemium
				solidwhole.draw(g);
				g.setColor(Color.black);
				g.drawString("Ingenium", 640, 300);

				if (textstate == 5) {
					allowContinue = false;
					openrocket.draw(g);
					g.setColor(Color.cyan);
					liquidfuelsplit = 1;
					liquidfuelselect = 1;
					solidfuelsplit = 1;
					g.fillRect(239, 382, 64, 92);
				}
			}

			if (textstate == 6) { //Added whole
				allowContinue = false;
				openrocket.draw(g);
				g.setColor(Color.white);
				g.fillOval(240, 275, 62, 62); //Add whole into open fuel
				g.setColor(Color.cyan);
				g.fillRect(239, 382, 64, 92);
				solidfuelselect = 1;
				speed = 18;
				launchstate = 1;
			}


			if (textstate == 7) { //Launch with full solid
				//gamestate = 1;
				launch();
				//attempts = 1;
			}

			if (textstate == 8) {
				allowContinue = true;
				liquidfuelsplit = 0;
				liquidfuelselect = 0;
				solidfuelsplit = 0;
				solidfuelselect = 0;
			}

			if (textstate == 9) { //Press 2 to split in half
				allowContinue = false;
			}

			if (textstate == 10) { //Split into half
				allowContinue = false;
				Sprite half = new Sprite("2-2", "2-2.png", new Point(500, 100));
				half.setScaleX(.75f);
				half.setScaleY(.75f);
				half.draw(g);
				solidfuelsplit = 2;
			}

			if (textstate == 11) { // Added half to rocket
				allowContinue = false;
				openrocket.draw(g);
				g.setColor(Color.cyan);
				g.fillRect(239, 382, 64, 92);
				Sprite one_two = new Sprite("1-2", "1-2.png", new Point(240, 275));
				one_two.setScaleX(.124f);
				one_two.setScaleY(.124f);
				Sprite subone_two = new Sprite("sub-1-2", "sub-1-2.png", new Point(500, 100));
				subone_two.setScaleX(.75f);
				subone_two.setScaleY(.75f);
				solidfuelselect = 1;
				solidfuelsplit = 2;
				liquidfuelsplit = 1;
				liquidfuelselect = 1;
				one_two.draw(g);
				subone_two.draw(g);
				speed = 25;
				launchstate = 1;
			}

			if (textstate == 12) { //Launch with half
				//gamestate = 1;
				launch();
				//	attempts=2;
			}

			if (textstate == 13) { //One frame to reset solid select
				solidfuelselect = 0;
				solidfuelsplit = 0;
				textstate++;
			}

			if (textstate == 14 || textstate == 23) { //Insert Number and Launch
				openrocket.draw(g);

				if(textstate == 14) {
					g.setColor(Color.cyan);
					g.fillRect(239, 382, 64, 92);
					allowContinue = false;
				}

				if (solidfuelsplit == 2) {

					if (solidfuelselect == 0) {
						Sprite half = new Sprite("2-2", "2-2.png", new Point(500, 100));
						half.setScale(.75f);
						half.draw(g);
					}

					if (solidfuelselect == 1) {
						Sprite one_two = new Sprite("1-2", "1-2.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-1-2.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 2) {
						Sprite half = new Sprite("2-2", "2-2.png", new Point(240, 275));
						half.setScale(.124f);
						half.draw(g);
					}
				}


				if (solidfuelsplit == 3) {

					if (solidfuelselect == 0) {
						Sprite half = new Sprite("2-2", "3-3.png", new Point(500, 100));
						half.setScale(.75f);
						half.draw(g);
					}

					if (solidfuelselect == 1) {
						Sprite one_two = new Sprite("1-2", "1-3.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-1-3.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 2) {
						Sprite one_two = new Sprite("1-2", "2-3.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-2-3.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 3) {
						Sprite half = new Sprite("2-2", "3-3.png", new Point(240, 275));
						half.setScale(.124f);
						half.draw(g);
					}
				}

				if (solidfuelsplit == 4) {

					if (solidfuelselect == 0) {
						Sprite half = new Sprite("2-2", "4-4.png", new Point(500, 100));
						half.setScale(.75f);
						half.draw(g);
					}

					if (solidfuelselect == 1) {
						Sprite one_two = new Sprite("1-2", "1-4.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-1-4.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 2) {
						Sprite one_two = new Sprite("1-2", "1-2.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-2-4.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 3) {
						Sprite one_two = new Sprite("1-2", "3-4.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-3-4.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 4) {
						Sprite half = new Sprite("2-2", "4-4.png", new Point(240, 275));
						half.setScale(.124f);
						half.draw(g);
					}
				}


				if (solidfuelsplit == 5) {

					if (solidfuelselect == 0) {
						Sprite half = new Sprite("2-2", "5-5.png", new Point(500, 100));
						half.setScale(.75f);
						half.draw(g);
					}

					if (solidfuelselect == 1) {
						Sprite one_two = new Sprite("1-2", "1-5.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-1-5.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 2) {
						Sprite one_two = new Sprite("1-2", "2-5.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-2-5.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 3) {
						Sprite one_two = new Sprite("1-2", "3-5.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-3-5.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 4) {
						Sprite one_two = new Sprite("1-2", "4-5.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-4-5.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 5) {
						Sprite half = new Sprite("2-2", "5-5.png", new Point(240, 275));
						half.setScale(.124f);
						half.draw(g);
					}
				}


				if (solidfuelsplit == 6) {

					if (solidfuelselect == 0) {
						Sprite half = new Sprite("2-2", "6-6.png", new Point(500, 100));
						half.setScale(.75f);
						half.draw(g);
					}

					if (solidfuelselect == 1) {
						Sprite one_two = new Sprite("1-2", "1-6.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-1-6.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 2) {
						Sprite one_two = new Sprite("1-2", "1-3.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-2-6.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 3) {
						Sprite one_two = new Sprite("1-2", "1-2.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-3-6.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 4) {
						Sprite one_two = new Sprite("1-2", "2-3.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-4-6.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 5) {
						Sprite one_two = new Sprite("1-2", "5-6.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-5-6.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 6) {
						Sprite half = new Sprite("2-2", "6-6.png", new Point(240, 275));
						half.setScale(.124f);
						half.draw(g);
					}
				}


				if (solidfuelsplit == 7) {

					if (solidfuelselect == 0) {
						Sprite half = new Sprite("2-2", "7-7.png", new Point(500, 100));
						half.setScale(.75f);
						half.draw(g);
					}

					if (solidfuelselect == 1) {
						Sprite one_two = new Sprite("1-2", "1-7.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-1-7.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 2) {
						Sprite one_two = new Sprite("1-2", "2-7.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-2-7.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 3) {
						Sprite one_two = new Sprite("1-2", "3-7.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-3-7.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 4) {
						Sprite one_two = new Sprite("1-2", "4-7.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-4-7.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 5) {
						Sprite one_two = new Sprite("1-2", "5-7.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-5-7.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 6) {
						Sprite one_two = new Sprite("1-2", "6-7.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-6-7.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 7) {
						Sprite half = new Sprite("2-2", "7-7.png", new Point(240, 275));
						half.setScale(.124f);
						half.draw(g);
					}
				}


				if (solidfuelsplit == 8) {

					if (solidfuelselect == 0) {
						Sprite half = new Sprite("2-2", "8-8.png", new Point(500, 100));
						half.setScale(.75f);
						half.draw(g);
					}

					if (solidfuelselect == 1) {
						Sprite one_two = new Sprite("1-2", "1-8.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-1-8.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 2) {
						Sprite one_two = new Sprite("1-2", "1-4.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-2-8.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 3) {
						Sprite one_two = new Sprite("1-2", "3-8.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-3-8.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 4) {
						Sprite one_two = new Sprite("1-2", "1-2.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-4-8.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 5) {
						Sprite one_two = new Sprite("1-2", "5-8.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-5-8.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 6) {
						Sprite one_two = new Sprite("1-2", "3-4.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-6-8.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 6) {
						Sprite one_two = new Sprite("1-2", "7-7.png", new Point(240, 275));
						one_two.setScale(.124f);
						one_two.draw(g);
						Sprite subone_two = new Sprite("sub-1-2", "sub-7-8.png", new Point(500, 100));
						subone_two.setScale(.75f);
						subone_two.draw(g);
					}

					if (solidfuelselect == 7) {
						Sprite half = new Sprite("2-2", "8-8.png", new Point(240, 275));
						half.setScale(.124f);
						half.draw(g);
					}
				}

				if (solidfuelselect > 0 && liquidfuelselect > 0) launchstate = 1;
			}

			if (textstate == 15) { //One frame: Used to iterate speed once
				allowContinue = false;
				//attempts++;
				if (((float) solidfuelselect / solidfuelsplit > .2f) && ((float) solidfuelselect / solidfuelsplit <= .4f))
					speed = 33; //1/3 or 2/7 or 2/5 or 1/4 or 3/8s
				else speed = 27;
				textstate++;
			}

			if (textstate == 16) //Launches
				//gamestate = 1;
				launch();

			if (textstate == 17) //If didn't make it
				allowContinue = false;

			if (textstate == 18) { //Made it
				g.setColor(Color.cyan);
				g.fillRect(500, 20, 375, 540);
				g.setColor(Color.black);
				g.drawString("Water", 638, 290);
				solidfuelsplit = 0;
				solidfuelselect = 0;
			}

			if (textstate == 19) { //Adding 1/3rds of water
				openrocket.draw(g);
				liquidfuelselect = 0;
				liquidfuelsplit = 0;
				solidfuelsplit = 3;
				if (solidfuelselect == 0) {
					Sprite half = new Sprite("2-2", "3-3.png", new Point(500, 100));
					half.setScale(.75f);
					half.draw(g);
					allowContinue = false;
				}

				if (solidfuelselect == 1) {
					Sprite one_two = new Sprite("1-2", "1-3.png", new Point(240, 275));
					one_two.setScale(.124f);
					one_two.draw(g);
					Sprite subone_two = new Sprite("sub-1-2", "sub-1-3.png", new Point(500, 100));
					subone_two.setScale(.75f);
					subone_two.draw(g);
				}

				if (solidfuelselect == 2) {
					Sprite one_two = new Sprite("1-2", "2-3.png", new Point(240, 275));
					one_two.setScale(.124f);
					one_two.draw(g);
					Sprite subone_two = new Sprite("sub-1-2", "sub-2-3.png", new Point(500, 100));
					subone_two.setScale(.75f);
					subone_two.draw(g);
				}

				if (solidfuelselect == 3) {
					Sprite one_two = new Sprite("2-2", "3-3.png", new Point(240, 275));
					one_two.setScale(.124f);
					one_two.draw(g);
				}
			}

			if (textstate == 20) { //Enter 1/4th of liquid
				allowContinue = false;
				liquidopenrocket.draw(g);
				solidfuelselect = 1;
				solidfuelsplit = 3;
				liquidfuelsplit = 4;
				if (liquidfuelselect == 0) {
					g.setColor(Color.cyan);
					g.fillRect(500, 20, 375, 540);
					g.setColor(Color.black);
					g.drawLine(500, 155, 875, 155);
					g.drawLine(500, 290, 875, 290);
					g.drawLine(500, 425, 875, 425);
				}
				if (liquidfuelselect == 1) {
					g.setColor(Color.cyan);
					g.fillRect(500, 155, 375, 405);
					g.fillRect(239, 451, 64, 23);
					g.setColor(Color.black);
					g.drawLine(500, 290, 875, 290);
					g.drawLine(500, 425, 875, 425);
				}

				if (liquidfuelselect == 2) {
					g.setColor(Color.cyan);
					g.fillRect(500, 290, 375, 270);
					g.fillRect(239, 428, 64, 46);
					g.setColor(Color.black);
					g.drawLine(500, 425, 875, 425);
					launchstate = 1;
				}
				speed = 36;
			}

			if (textstate == 21) { //Launches
				//gamestate = 1;
				launch();
			}

			if (textstate == 22) { //One frame, reset solid and liquid
				solidfuelselect = 0;
				solidfuelsplit = 0;
				liquidfuelselect = 0;
				liquidfuelsplit = 0;
				textstate++;
			}

			if (textstate == 23) { //Adding solid
				if (solidfuelselect == 0) allowContinue = false;
			}


			if (textstate == 24) { //Adding liquid
				allowContinue = false;
				liquidopenrocket.draw(g);
				if (liquidfuelselect > 0) launchstate = 1;

				g.setColor(Color.cyan);
				int splits = liquidfuelsplit - liquidfuelselect;
				int space = 0;

				for(int i = splits; i > 0; i--){
					g.fillRect(500, 560 + space - (540 / liquidfuelsplit) * i, 375, (540 / liquidfuelsplit));
					space+=2;
				}

				for(int i = liquidfuelselect; i>0; i--){
					g.fillRect(239, 473 - (92 / liquidfuelsplit) * i, 64, (92 / liquidfuelsplit));
				}
			}

			if (textstate == 25) { //One frame used for speed
				if (((float) solidfuelselect / solidfuelsplit > .2f) && ((float) solidfuelselect / solidfuelsplit <= .4f)
						&& ((float) liquidfuelselect / liquidfuelsplit > .2f) && ((float) liquidfuelselect / liquidfuelsplit <= .4f))
					speed = 40; //1/3 or 2/7 or 2/5 or 1/4 or 3/8
				else speed = 37;
				textstate++;
			}

			if (textstate == 26) { //Launching
				//gamestate = 1;
				launch();
			}

			if (textstate == 27) //Didn't make it to the moon
				allowContinue = false;

			if (textstate == 28) //Made it to the moon
				allowContinue = false;


//----------------------------------------------------------------------------------------------------------------------

			if (gamestate == 1) { //If launched state

				allowContinue = false;

				if (rocket != null && rocket.getVisible() == 1) {
					rocket.draw(g);
					rocket.animate("launch");
				}

				if (background.getPosition().y + speed <= -20 && background.getPosition().y + speed >= -3950) {
					background.setPosition(new Point(0, background.getPosition().y + Math.round(speed)));
					if (speed < 0) {
						rocket.stopAnimation(0);
						sounds.StopSound("launched");
					}

					if (speed > -60)
						speed -= gravity;
				} else {
					//System.out.println("end me");
					rocket.stopAnimation(0);
					sounds.StopSound("launched");

					if (background.getPosition().y < -1000) { //Didn't make it
						gamestate = 0;
						background.setPosition(new Point(0, -3950));
						speed = 0;

						if (textstate == 16 && maxheight > 1900) textstate += 2; //If made it to the stars

						else
							textstate++;

					}

					else {
						gamestate = -1;
						if (textstate == 26 && maxheight > 3400) textstate += 2; //If made it to the moon
					}
					// The End
					//System.out.println(rocket.getCurrentFrame());
				}


			}
		}



	}

	private ArrayList<Integer> init_checkpoints() {
		ArrayList<Integer> list = new ArrayList<>();

		list.add(5);
		list.add(6);
		list.add(10);

		return list;
	}


	public static void main(String[] args) {
		ToSpace game = new ToSpace();
		game.start();

	}
}
