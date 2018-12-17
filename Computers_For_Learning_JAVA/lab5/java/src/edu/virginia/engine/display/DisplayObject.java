package edu.virginia.engine.display;

import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.awt.AlphaComposite;
import java.awt.Point;
import java.awt.Rectangle;


import javax.imageio.ImageIO;

/**
 * A very basic display object for a java based gaming engine
 * 
 * */

/* Names: Zack Crenshaw & Ni'Gere Epps */

public class DisplayObject extends SoundManager{

	/* All DisplayObject have a unique id */
	private String id;

	/* The image that is displayed by this object */
	private BufferedImage displayImage;

	private int visible; //true iff object is meant to be drawn

	private float alpha; //alpha channel value for drawing

	private float oldAlpha; //old alpha channel value

	private float scaleX; //scale factor on X axis

	private float scaleY; //scale factor on Y axis

	private Point position; //where object is drawn

	private Point pivotPoint; //origin of object

	private float Rotation; //degrees of rotation

	private Shape hitbox;

	private boolean hasPhysics;

	private int speed;

	private int mass;

	private int falling;

	private int collided;


	/**
	 * Constructors: can pass in the id OR the id and image's file path and
	 * position OR the id and a buffered image and position
	 */
	public DisplayObject(String id) {
		this.setId(id);
		this.setVisible(1);
		this.setAlpha(1);
		this.setOldAlpha(1);
		this.setScaleX(1);
		this.setScaleY(1);
		this.setPosition(new Point(0,0));
		this.setPivotPoint(new Point(0,0));
		this.setRotation(0);
		this.hitbox = new Rectangle(0,0, (int)(this.getUnscaledWidth() * this.getScaleX()),
												(int)(this.getUnscaledHeight() * this.getScaleY()));
		this.hasPhysics = true;
		this.falling = 0;

		this.setSpeed(1);
		this.setMass(1);
		this.resetCollided();
	}

	public DisplayObject(String id, String fileName) {
		this.setId(id);
		this.setImage(fileName);
		this.setVisible(1);
		this.setAlpha(1);
		this.setOldAlpha(1);
		this.setScaleX(1);
		this.setScaleY(1);
		this.setPosition(new Point(0,0));
		this.setPivotPoint(new Point(0,0));
		this.setRotation(0);
		this.hitbox = new Rectangle(0,0, (int)(this.getUnscaledWidth() * this.getScaleX()),
												(int)(this.getUnscaledHeight() * this.getScaleY()));
		this.hasPhysics = true;
		this.falling = 0;
		this.setSpeed(1);
		this.setMass(1);
		this.resetCollided();
	}

	public Shape getHitbox(){
		return this.hitbox;
	}

	public void setHitbox(Shape box){
		this.hitbox = box;
	}

	public boolean collidesWith(DisplayObject other){
		return this.hitbox.intersects(other.getHitbox().getBounds());
	}

	public void setId(String id) {
		this.id = id;
	}

	public String getId() {
		return id;
	}

	public boolean getHasPhyisics() {return this.hasPhysics;}

	public void setHasPhysics(boolean b) {this.hasPhysics = b;}

	public int getSpeed() {return this.speed;}

	public void setSpeed(int s) { this.speed = s;}

	public int getMass() {return this.mass;}

	public void setMass(int m) {this.mass = m;}

	public void setFalling(int f) {this.falling = f;}

	public void incFalling(int i) {this.falling += i;}

	public void incFalling() {this.falling++;}

	public int getFalling() {return this.falling;}

	public void setCollided(int i) {this.collided = i;}

	public void resetCollided() {this.collided = 0;}

	public void incCollided() {this.collided++;}

	public int getCollided() {return this.collided;}

	public void updateHitbox(){
		AffineTransform tx = new AffineTransform();

		tx.rotate(Math.toRadians(this.getRotation()),
				(int)(this.getPivotPoint().getX() + this.getPosition().getX()),
				(int)(this.getPivotPoint().getY() + this.getPosition().getY()));

		Shape shape = new Rectangle((int)this.getPosition().getX(), (int)this.getPosition().getY(),
							    	(int)(this.getUnscaledWidth() * this.getScaleX()),
									(int)(this.getUnscaledHeight() * this.getScaleY()));

		Shape newshape = tx.createTransformedShape(shape);
		this.hitbox = newshape;
		//System.out.println("Mario's height: "+this.getUnscaledHeight()*this.getScaleX());

	}


	/**
	 * Returns the unscaled width and height of this display object
	 * */
	public int getUnscaledWidth() {
		if(displayImage == null) return 0;
		return displayImage.getWidth();
	}

	public int getUnscaledHeight() {
		if(displayImage == null) return 0;
		return displayImage.getHeight();
	}

	public BufferedImage getDisplayImage() {
		return this.displayImage;
	}

	protected void setImage(String imageName) {
		if (imageName == null) {
			return;
		}
		displayImage = readImage(imageName);
		if (displayImage == null) {
			System.err.println("[DisplayObject.setImage] ERROR: " + imageName + " does not exist!");
		}
	}


	/**
	 * Helper function that simply reads an image from the given image name
	 * (looks in resources\\) and returns the bufferedimage for that filename
	 * */
	public BufferedImage readImage(String imageName) {
		BufferedImage image = null;
		try {
			String file = ("resources" + File.separator + imageName);
			image = ImageIO.read(new File(file));
		} catch (IOException e) {
			System.out.println("[Error in DisplayObject.java:readImage] Could not read image " + imageName);
			e.printStackTrace();
		}
		return image;
	}

	public void setImage(BufferedImage image) {
		if(image == null) return;
		displayImage = image;
	}


	/**
	 * Invoked on every frame before drawing. Used to update this display
	 * objects state before the draw occurs. Should be overridden if necessary
	 * to update objects appropriately.
	 * */
	protected void update(ArrayList<Integer> pressedKeys) {
		
	}

	/**
	 * Draws this image. This should be overloaded if a display object should
	 * draw to the screen differently. This method is automatically invoked on
	 * every frame.
	 * */
	public void draw(Graphics g) {
		
		if (displayImage != null) {
			/*
			 * Get the graphics and apply this objects transformations
			 * (rotation, etc.)
			 */
			Graphics2D g2d = (Graphics2D) g;
			applyTransformations(g2d);
			/* Actually draw the image, perform the pivot point translation here */

		//	g2d.draw(this.hitbox);
			g2d.drawImage(displayImage,
					0,
					0,
					(int) (getUnscaledWidth()),
					(int) (getUnscaledHeight()), null);


			
			//g2d.draw(this.hitbox);
			/*
			 * undo the transformations so this doesn't affect other display
			 * objects
			 */
			reverseTransformations(g2d);

			//g2d.draw(this.hitbox);
		}
	}

	/**
	 * Applies transformations for this display object to the given graphics
	 * object
	 * */
	protected void applyTransformations(Graphics2D g2d) {
		g2d.translate(this.position.x, this.position.y);
		g2d.rotate(Math.toRadians(this.Rotation), this.pivotPoint.x, this.pivotPoint.y);
		g2d.scale(this.scaleX, this.scaleY);
		float curAlpha;
		this.oldAlpha = curAlpha = ((AlphaComposite)
				g2d.getComposite()).getAlpha();
		g2d.setComposite(AlphaComposite.getInstance(3, curAlpha *
				this.alpha));
	}

	/**
	 * Reverses transformations for this display object to the given graphics
	 * object
	 * */
	protected void reverseTransformations(Graphics2D g2d) {
		g2d.setComposite(AlphaComposite.getInstance(3, this.oldAlpha));
		g2d.scale(1.0/this.scaleX,1.0/this.scaleY);
		g2d.rotate(Math.toRadians(360 - this.Rotation), -this.pivotPoint.x, -this.pivotPoint.y);
		g2d.translate(-this.position.x,-this.position.y);
	}

	public Point getPosition(){
		return this.position;
	}

	public void setPosition(Point pst){
		this.position = pst;
	}

	public Point getPivotPoint() {
		return this.pivotPoint;
	}

	public void setPivotPoint(Point pvt){
		this.pivotPoint = pvt;
	}

	public float getRotation(){
		return this.Rotation;
	}

	public void setRotation(float rot){
		this.Rotation = rot;
	}


	/* Setters and getters for these new variables */

	public int getVisible() {
		return this.visible;
	}

	public void setVisible(int v) {
		this.visible = v;
	}

	public void switchVisible() {
	    this.visible = this.visible*-1;
    }

	public float getAlpha() {
		return alpha;
	}
	
	public void setAlpha(float a) {
		this.alpha = a;
	}

	public void incAlpha() {
	    if (this.alpha < 0.909) {
	        this.alpha = (this.alpha*1.1f);
        }
        else {
            this.alpha = 1;
        }
    }

    public void decAlpha() {
	    this.alpha = this.alpha * 0.909f;
    }

	public float getOldAlpha() {
		return this.oldAlpha;
	}

	public void setOldAlpha(float a) {
		this.oldAlpha = a;
	}

	public float getScaleX() {
		return this.scaleX;
	}

	public void setScaleX(float sx) {
		this.scaleX = sx;
	}

	public float getScaleY() {
		return this.scaleY;
	}

	public void setScaleY(float sy) {
		this.scaleY = sy;
	}





}
