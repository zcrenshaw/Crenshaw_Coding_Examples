package edu.virginia.engine.display;

public class Animation {

    private String id;          //Name of animation
    private int startFrame;     //Index of the start frame in the list of images
    private int endFrame;       //Index of the last frame in the list of images

    public Animation(String name, int start, int end){
        this.id = name;
        this.startFrame = start;
        this.endFrame = end;
    }

    public String getId(){
        return this.id;
    }

    public void setId(String id){
        this.id = id;
    }

    public int getStartFrame(){
        return this.startFrame;
    }

    public void setStartFrame(int startFrame){
        this.startFrame = startFrame;
    }

    public int getEndFrame(){
        return this.endFrame;
    }

    public void setEndFrame(int endFrame){
        this.endFrame = endFrame;
    }

}


