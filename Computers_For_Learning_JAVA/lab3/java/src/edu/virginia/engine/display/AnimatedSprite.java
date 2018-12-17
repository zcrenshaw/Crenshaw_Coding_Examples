package edu.virginia.engine.display;

import edu.virginia.engine.util.GameClock;

import java.awt.image.BufferedImage;
import java.util.ArrayList;
import java.awt.Point;
import java.awt.Graphics;


public class AnimatedSprite extends Sprite {

    private ArrayList<Animation> animations;   //Animation objects
    private boolean playing;            //If it's being animated
    private String fileName;        //file that contains images for animation
    private ArrayList<BufferedImage> frames;       //Images to draw for animation
    private int currentFrame;      //Index of image currently being drawn
    private int startFrame;         //Index of first image drawn
    private int endFrame;           //Index of end image
    static final int DEFAULT_ANIMATION_SPEED = 100;   //default speed at which images change
    private int animationSpeed;     //Speed at which images change
    private GameClock gameClock;    //GameClock for this Sprite


    public AnimatedSprite(String id, String filename, Point pos) {
        super(id, filename);
        super.setPosition(pos);

        this.animationSpeed = DEFAULT_ANIMATION_SPEED;
        this.playing = false;

        Animation left = new Animation("left",1,3);
        Animation right = new Animation("right",4,6);

        this.frames = new ArrayList<BufferedImage>();
        this.initFrames();

        this.animations = new ArrayList<Animation>();
        this.animations.add(left);
        this.animations.add(right);

        this.currentFrame = 0;
        this.startFrame = 0;
        this.endFrame = 0;
        this.gameClock = new GameClock();

     //   this.setScaleX(2);
     //   this.setScaleY(2);

    }

    public void setStartFrame(int startFrame) {
        this.startFrame = startFrame;
    }

    public void setEndFrame(int endFrame) {
        this.endFrame = endFrame;
    }

    public void setCurrentFrame(int currentFrame) {
        this.currentFrame = currentFrame;
    }

    public int getCurrentFrame(){
        return this.currentFrame;
    }

    public void initGameClock() {
        if(this.gameClock == null)
            this.gameClock = new GameClock();
    }

    public void setAnimations(ArrayList<Animation> ams){
        this.animations = ams;
    }

    public void setAnimationSpeed(int speed){
        this.animationSpeed = speed;
    }

    public int getAnimationSpeed(){ return this.animationSpeed; }

    @Override
    public void draw(Graphics g) {

        while(this.playing && this.gameClock.getElapsedTime() >= this.animationSpeed) {

            this.setImage(this.frames.get(this.currentFrame));
            this.currentFrame++;

            if(this.currentFrame > this.endFrame) {
                     this.currentFrame = this.startFrame;
            }

            this.gameClock.resetGameClock();

        }

        super.draw(g);

    }


    public void initFrames(){
        /* call readImage to populate index
            manipulate strings to populate the whole list
         */
/*
        String rl = "left_";
        int addme = 1;
        for (int i = 0; i < 6; i++) {
            if (i>2) {
                rl = "right_";
            }
            if (i == 3) {
                addme = 2;
            }
            String ending = Integer.toString((i+addme)%4);
            System.out.println("mario_run_" + rl + ending + ".png");
            String file = "mario_run_" + rl + ending + ".png";
            this.frames.add(this.readImage(file));
        }
*/

        this.frames.add(this.readImage("Mario.png"));
        this.frames.add(this.readImage("mario_run_left_1.png"));
        this.frames.add(this.readImage("mario_run_left_2.png"));
        this.frames.add(this.readImage("mario_run_left_3.png"));
        this.frames.add(this.readImage("mario_run_right_1.png"));
        this.frames.add(this.readImage("mario_run_right_2.png"));
        this.frames.add(this.readImage("mario_run_right_3.png"));

    }

    public Animation getAnimation(String id) { //get animation with id
        for (Animation a : this.animations) {
            if (a.getId().equals(id)) {
                return a;
            }
        }

        return null;
    }

    public void animate(Animation a) { //start animation from a given animation
        this.setStartFrame(a.getStartFrame());
        this.setEndFrame(a.getEndFrame());
        this.setCurrentFrame(a.getStartFrame());
        this.playing = true;

    }

    public void animate(String id) { //find animation with id and begin animation there

        for (Animation a : this.animations) {
            if (a.getId().equals(id)) {
                this.playing = true;
                this.setStartFrame(a.getStartFrame());
                this.setEndFrame(a.getEndFrame());
                this.setCurrentFrame(a.getStartFrame());

            }

        }

    }

    public void animate(int startFrame, int endFrame) { //start animation at a given start and end frame
        this.setCurrentFrame(startFrame);
        this.setStartFrame(startFrame);
        this.setEndFrame(endFrame);
        this.playing = true;
    }

    public void stopAnimation(int frameNumber) {

            this.currentFrame = frameNumber;
            this.playing = false;

    }

    public void stopAnimation() {
        stopAnimation(0);

    }

}

