package edu.virginia.lab3test;

import java.awt.Graphics;
import java.util.ArrayList;
import java.awt.event.KeyEvent;
import java.awt.Point;

import edu.virginia.engine.display.*;

/**
 * Example game that utilizes our engine. We can create a simple prototype game with just a couple lines of code
 * although, for now, it won't be a very fun game :)
 * */

// Names: Ni'Gere Epps & Zack Crenshaw
public class LabThreeSimulator extends Game{

	/*
	DisplayObjectContainer sun = new DisplayObjectContainer("sun", "sun.png");
	DisplayObjectContainer mars = new DisplayObjectContainer("mars", "mars.png");
	DisplayObjectContainer earth = new DisplayObjectContainer("earth", "earth.png");
	*/

	Sprite sun = new Sprite("sun", "sun.png");
	Sprite mars = new Sprite("mars", "mars.png");
	Sprite earth = new Sprite("earth", "earth.png");

	DisplayObject moon1 = new DisplayObject("moon1", "moon.png");
	DisplayObject moon2 = new DisplayObject("moon2", "moon.png");

	int direction = 1;
	
	/**
	 * Constructor. See constructor in Game.java for details on the parameters given
	 * */
	public LabThreeSimulator() {

		super("Lab Three Test Game", 800, 800);

		sun.addChild(earth);
		sun.addChild(mars);
		earth.addChild(moon1);
		mars.addChild(moon2);

		sun.setPosition(new Point(155,  155));
		earth.setPosition(new Point(50,0));
		mars.setPosition(new Point(250,0));
		moon1.setPosition(new Point( 100, 50));
		moon2.setPosition(new Point(200,50));

		moon1.setScaleX(0.5f);
		moon1.setScaleY(0.5f);
		moon2.setScaleX(0.5f);
		moon2.setScaleY(0.5f);

	}
	
	/**
	 * Engine will automatically call this update method once per frame and pass to us
	 * the set of keys (as strings) that are currently being pressed down
	 * */
	@Override
	public void update(ArrayList<Integer> pressedKeys){
		super.update(pressedKeys);
		
		/* Make sure sun is not null. Sometimes Swing can auto cause an extra frame to go before everything is initialized */

		if(sun != null) sun.update(pressedKeys);

		//setting pivotpoints for rotation

		sun.setPivotPoint(new Point((int)((sun.getPosition().getX() + 66) * sun.getScaleX()),
									(int)((sun.getPosition().getY() + 66) * sun.getScaleY())));

		earth.setPivotPoint(new Point ( (int)(sun.getPosition().getX() - earth.getPosition().getX()) + 66,
										(int)(sun.getPosition().getY() - earth.getPosition().getY()) + 66));

		mars.setPivotPoint(new Point ((int)(sun.getPosition().getX() - mars.getPosition().getX()) + 66,
									  (int)(sun.getPosition().getY() - mars.getPosition().getY()) + 66));

		moon1.setPivotPoint(new Point ( (int)(earth.getPosition().getX() - moon1.getPosition().getX()) + 33,
										(int)(earth.getPosition().getY() - moon1.getPosition().getY()) + 33));

		moon2.setPivotPoint(new Point ( (int)(mars.getPosition().getX() - moon2.getPosition().getX()) + 33,
										(int)(mars.getPosition().getY() - moon2.getPosition().getY()) + 33));

//setting rotations
		earth.setRotation(earth.getRotation() + direction*2);
		mars.setRotation(mars.getRotation() + direction);
		moon1.setRotation(moon1.getRotation() + direction * 3);
		moon2.setRotation(moon2.getRotation() + direction * 3);
		sun.setRotation(sun.getRotation() + direction);

//key events
		if (pressedKeys.contains(KeyEvent.VK_W)) {
			sun.setScaleX(sun.getScaleX() * 0.909f);
			sun.setScaleY(sun.getScaleY() * 0.909f);
		}

		if (pressedKeys.contains(KeyEvent.VK_Q)) {
			sun.setScaleX(sun.getScaleX() * 1.1f);
			sun.setScaleY(sun.getScaleY() * 1.1f);
		}

		if (pressedKeys.contains(KeyEvent.VK_A)) {

			direction = -1;
		}

		if (pressedKeys.contains(KeyEvent.VK_S)) {

			direction = 1;
		}


		if (pressedKeys.contains(KeyEvent.VK_DOWN)) {
			sun.setPosition(new Point(sun.getPosition().x,
					sun.getPosition().y - 1));
		}

		if (pressedKeys.contains(KeyEvent.VK_UP)) {
			sun.setPosition(new Point(sun.getPosition().x,
					sun.getPosition().y + 1 ));
		}

		if (pressedKeys.contains(KeyEvent.VK_RIGHT)) {
			sun.setPosition(new Point(sun.getPosition().x - 1,
					sun.getPosition().y));

		}

		if (pressedKeys.contains(KeyEvent.VK_LEFT)) {
			sun.setPosition(new Point(sun.getPosition().x + 1,
					sun.getPosition().y));
		}


	}
	
	/**
	 * Engine automatically invokes draw() every frame as well. If we want to make sure mario gets drawn to
	 * the screen, we need to make sure to override this method and call mario's draw method.
	 * */
	@Override
	public void draw(Graphics g){
		super.draw(g);
		
		/* Same, just check for null in case a frame gets thrown in before Sun is initialized */
		if(sun != null) sun.draw(g);

	}

	/**
	 * Quick main class that simply creates an instance of our game and starts the timer
	 * that calls update() and draw() every frame
	 * */
	public static void main(String[] args) {
		LabThreeSimulator game = new LabThreeSimulator();
		game.start();

	}
}
