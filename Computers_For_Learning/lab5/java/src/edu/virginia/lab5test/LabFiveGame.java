package edu.virginia.lab5test;

import java.awt.Graphics;
import java.util.ArrayList;
import java.awt.event.KeyEvent;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.*;

import edu.virginia.engine.display.*;


/**
 * Example game that utilizes our engine. We can create a simple prototype game with just a couple lines of code
 * although, for now, it won't be a very fun game :)
 * */

// Names: Ni'Gere Epps & Zack Crenshaw
public class LabFiveGame extends Game {

	/* Create a sprite object for our game. We'll use mario */
	AnimatedSprite mario = new AnimatedSprite("Mario", "Mario.png", new Point(0,0));
	Sprite obstacle = new Sprite("piranha_plant", "piranha_plant.png");
	Sprite goal = new Sprite("star", "star.png");
	int score = 1000;
	SoundManager sounds = new SoundManager();
	int gravity = 1;
	int maxFalling = 15;


	/**
	 * Constructor. See constructor in Game.java for details on the parameters given
	 * */
	public LabFiveGame() {

		super("Lab Four Test Game", 1200, 800);

		mario.setScaleX(2);
		mario.setScaleY(2);

		obstacle.setPosition(new Point(300,364));

		obstacle.setScaleX(2); obstacle.setScaleY(2);

		//obstacle.setHitbox(new Rectangle(100 , 100, obstacle.getUnscaledWidth(), obstacle.getUnscaledHeight()));
		obstacle.setHitbox(new Rectangle((int)obstacle.getPosition().getX() , (int)obstacle.getPosition().getY(),
				(int)(obstacle.getUnscaledWidth() * obstacle.getScaleX()),
				(int)(obstacle.getUnscaledHeight() * obstacle.getScaleY())));

		goal.setPosition(new Point(700,364));

		goal.setScaleX(2); goal.setScaleY(2);

		//goal.setHitbox(new Rectangle(175 , 175, goal.getUnscaledWidth(), goal.getUnscaledHeight()));

		goal.setHitbox(new Rectangle((int)goal.getPosition().getX() , (int)goal.getPosition().getY(),
				(int)(goal.getUnscaledWidth() * goal.getScaleX()),
				(int)(goal.getUnscaledHeight() * goal.getScaleY())));

		mario.setHitbox(new Rectangle((int)mario.getPosition().getX(),
				(int)mario.getPosition().getY(),
				(int)( mario.getUnscaledWidth() ),
				(int)(mario.getUnscaledHeight() )));

		sounds.LoadMusic("background","happy_mario.wav");
		sounds.LoadSoundEffect("win","win.wav");
		sounds.LoadSoundEffect("lose","lose.wav");
		sounds.LoadSoundEffect("collision_bad","collision_bad.wav");
		sounds.PlayMusic("background");



	}
	
	/**
	 * Engine will automatically call this update method once per frame and pass to us
	 * the set of keys (as strings) that are currently being pressed down
	 * */
	@Override
	public void update(ArrayList<Integer> pressedKeys){
		super.update(pressedKeys);
		
		/* Make sure mario is not null. Sometimes Swing can auto cause an extra frame to go before everything is initialized */
		if(mario != null) mario.update(pressedKeys);

		mario.incCollided();


		if (pressedKeys.contains(KeyEvent.VK_V)){
			mario.switchVisible();
			try
			{
				Thread.sleep(200);
			}
			catch(InterruptedException ex)
			{
				Thread.currentThread().interrupt();
			}

		}


		if (pressedKeys.contains(KeyEvent.VK_UP)) {
			mario.setFalling(0);
			if(mario.getPosition().getY() > 0)
			mario.setPosition(new Point(mario.getPosition().x,
					mario.getPosition().y - 5));
		}

		if (pressedKeys.contains(KeyEvent.VK_DOWN)) {
			if(mario.getPosition().getY() + mario.getUnscaledHeight() * mario.getScaleY() < 400)
			mario.setPosition(new Point(mario.getPosition().x,
					mario.getPosition().y + 5));
		}

		if (pressedKeys.contains(KeyEvent.VK_LEFT)) {
					mario.setPosition(new Point(mario.getPosition().x - mario.getSpeed(),
					mario.getPosition().y));
			mario.animate("left");
			if(mario.getSpeed() < 30 && mario.getHasPhyisics()) mario.setSpeed(mario.getSpeed() + 1);
			if(mario.getPosition().getX() < -mario.getUnscaledWidth()) mario.setPosition(new Point(1150, mario.getPosition().y));
		}

		if (pressedKeys.contains(KeyEvent.VK_RIGHT)) {
			mario.setPosition(new Point(mario.getPosition().x + mario.getSpeed(),
					mario.getPosition().y));
			mario.animate("right");
			if(mario.getSpeed() < 30 && mario.getHasPhyisics()) mario.setSpeed(mario.getSpeed() + 1);
			if(mario.getPosition().getX() > 1175) mario.setPosition(new Point(-10, mario.getPosition().y));
			//System.out.println(mario.getCurrentFrame());

		}

		if (pressedKeys.contains(KeyEvent.VK_I)) {
			mario.setPivotPoint(new Point(mario.getPivotPoint().x,
					mario.getPivotPoint().y - 5));
		}

		if (pressedKeys.contains(KeyEvent.VK_K)) {
			mario.setPivotPoint(new Point(mario.getPivotPoint().x,
					mario.getPivotPoint().y + 5));
		}

		if (pressedKeys.contains(KeyEvent.VK_J)) {
			mario.setPivotPoint(new Point(mario.getPivotPoint().x - 5,
					mario.getPivotPoint().y));
		}

		if (pressedKeys.contains(KeyEvent.VK_L)) {
			mario.setPivotPoint(new Point(mario.getPivotPoint().x + 5,
					mario.getPivotPoint().y));
		}

		if (pressedKeys.contains(KeyEvent.VK_W)) {
			mario.setRotation(mario.getRotation() + 1);
		}

		if (pressedKeys.contains(KeyEvent.VK_Q)) {
			mario.setRotation(mario.getRotation() - 1);
		}

		if (pressedKeys.contains(KeyEvent.VK_Z)) {
			mario.incAlpha();
		}

		if (pressedKeys.contains(KeyEvent.VK_X)) {
			mario.decAlpha();
		}

		if (pressedKeys.contains(KeyEvent.VK_A)) {
			if(mario.getUnscaledHeight() * mario.getScaleY() < 350) {
				mario.setScaleX(mario.getScaleX() * 1.01f);
				mario.setScaleY(mario.getScaleY() * 1.01f);
			}
			if(mario.getPosition().getY() + mario.getUnscaledHeight() * mario.getScaleY() > 400)
				mario.setPosition(new Point(mario.getPosition().x,
											(int) (390 - mario.getScaleY() * mario.getUnscaledHeight())));
		/*
			obstacle.setScaleX(obstacle.getScaleX()*1.1f);
			obstacle.setScaleY(obstacle.getScaleY()*1.1f);
			goal.setScaleX(goal.getScaleX()*1.1f);
			goal.setScaleY(goal.getScaleY()*1.1f);
		*/
		}

		if (pressedKeys.contains(KeyEvent.VK_S)) {
			mario.setScaleX(mario.getScaleX()*0.99f);
			mario.setScaleY(mario.getScaleY()*0.99f);
		/*
			obstacle.setScaleX(obstacle.getScaleX()*0.909f);
			obstacle.setScaleY(obstacle.getScaleY()*0.909f);
			goal.setScaleX(goal.getScaleX()*0.909f);
			goal.setScaleY(goal.getScaleY()*0.909f);
		*/
		}

		if (pressedKeys.contains(KeyEvent.VK_E)) {
			mario.setAnimationSpeed(mario.getAnimationSpeed() + 5);
		}

		if (pressedKeys.contains(KeyEvent.VK_R)) {
			mario.setAnimationSpeed(mario.getAnimationSpeed() - 5);
			if(mario.getAnimationSpeed() < 160)
				mario.setAnimationSpeed(mario.getAnimationSpeed() + 1);
		}

		if (pressedKeys.contains(KeyEvent.VK_R)) {
			if(mario.getAnimationSpeed() > 35)
				mario.setAnimationSpeed(mario.getAnimationSpeed() - 1);
		}

		if (pressedKeys.contains(KeyEvent.VK_F)) {
			mario.stopAnimation();
		}


		/* NAIVE GRAVITY:

		if (mario.getHasPhyisics()) {
			mario.setPosition((new Point(mario.getPosition().x,
					mario.getPosition().y + gravity)));
        }

        */

		//More complex gravity:

		if (mario.getHasPhyisics() && mario.getCollided() > 30) {
			if (mario.getFalling() < maxFalling) {
				mario.incFalling(gravity);
			}
			if(mario.getPosition().getY() < 400 - mario.getScaleY() * mario.getUnscaledHeight())
			mario.setPosition((new Point(mario.getPosition().x,
					mario.getPosition().y + mario.getFalling())));

		}

	/*	mario.setHitbox(new Rectangle((int)mario.getPosition().getX(),
				(int)mario.getPosition().getY(),
				(int)( mario.getUnscaledWidth() ),
				(int)(mario.getUnscaledHeight() ))); */
		mario.updateHitbox();
		//goal.updateHitbox();
		//obstacle.updateHitbox();
		if(pressedKeys.isEmpty()){
			if (mario.getHasPhyisics())
			mario.setSpeed(1);
		}

		if (pressedKeys.contains(KeyEvent.VK_N)) {
			if (mario.getMass() < 4)
			mario.setMass(mario.getMass() + 1);
		}

		if (pressedKeys.contains(KeyEvent.VK_M)) {
			if (mario.getMass() > 1)
			mario.setMass(mario.getMass() - 1);
		}

		//System.out.println(mario.getMass());

		mario.updateHitbox();

		//System.out.println(mario.getCollided());



	}
	
	/**
	 * Engine automatically invokes draw() every frame as well. If we want to make sure mario gets drawn to
	 * the screen, we need to make sure to override this method and call mario's draw method.
	 * */
	@Override
	public void draw(Graphics g){
		super.draw(g);
		
		/* Same, just check for null in case a frame gets thrown in before Mario is initialized */
		if(mario != null && mario.getVisible() == 1) mario.draw(g);
		if(obstacle != null && obstacle.getVisible() == 1) obstacle.draw(g);
		if(goal != null && goal.getVisible() == 1) goal.draw(g);

		g.setColor(Color.green);
		g.fillRect(0,400,1200,400);

		if(mario.collidesWith(obstacle)){

			if (mario.getHasPhyisics()) {

				if(mario.getPosition().getX() < obstacle.getPosition().getX())
					mario.setPosition(new Point(mario.getPosition().x - mario.getMass() * mario.getSpeed(),
												mario.getPosition().y));

				if(mario.getPosition().getX() > obstacle.getPosition().getX())
					mario.setPosition(new Point(mario.getPosition().x + mario.getMass() * mario.getSpeed(),
							mario.getPosition().y));

				/*if(mario.getPosition().getY() < obstacle.getPosition().getY())
					mario.setPosition(new Point(mario.getPosition().x,
							mario.getPosition().y - mario.getMass() * mario.getSpeed()));

				if(mario.getPosition().getY() > obstacle.getPosition().getY())
					mario.setPosition(new Point(mario.getPosition().x,
							mario.getPosition().y + mario.getMass() * mario.getSpeed()));
				*/
			}
			score-= 5;

			mario.resetCollided();
			sounds.PlaySoundEffect("collision_bad");
			/*try
			{
				Thread.sleep(200);
			}
			catch(InterruptedException ex)
			{
				Thread.currentThread().interrupt();
			}
*/


		}
		g.setColor(Color.black);

		if(score==0) {
			g.drawString("You Lose!", 200, 100);
			sounds.StopSound("background");
			sounds.PlaySoundEffect("lose");
			stop();
		}

		if(mario.collidesWith(goal)){
			g.drawString("You Win!", 200,100);
			sounds.StopSound("background");
			sounds.PlaySoundEffect("win");
			stop();
		}

		g.drawString("Score: " + this.score, 200,50);
	}

	/**
	 * Quick main class that simply creates an instance of our game and starts the timer
	 * that calls update() and draw() every frame
	 * */
	public static void main(String[] args) {
		LabFiveGame game = new LabFiveGame();
		game.start();

	}
}
