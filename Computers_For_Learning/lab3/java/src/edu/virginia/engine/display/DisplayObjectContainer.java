package edu.virginia.engine.display;

import java.awt.*;
import java.util.ArrayList;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;


public class DisplayObjectContainer extends DisplayObject {

    /* List of children */

    private ArrayList<DisplayObject> children;


    /* constructors */


    public DisplayObjectContainer(String id) {
        super(id);
        this.children = new ArrayList<DisplayObject>();

    }

    public DisplayObjectContainer(String id, String fileName) {
        super(id, fileName);
        this.children = new ArrayList<DisplayObject>();

    }

    /* list manipulation */

    public void addChild(DisplayObject child) {
        this.children.add(child);
        child.setParent(this);
    }

    public void addChildAtIndex(DisplayObject child, int index) {
        this.children.add(index,child);
        child.setParent(this);
    }

    public void removeChild(DisplayObject child) {
       this.children.remove(child);
    }

    public void removeByIndex(int index) {
        this.children.remove(index);
    }

    public void removeAll() {
        this.children.clear();
    }


    public DisplayObject getChild(String id){
        for(DisplayObject c : this.children){
            if(c.getId().equals(id)){
                return c;
            }
        }
        return null;
    }

    public DisplayObject getChild(int index){
        return this.children.get(index);
    }

    public ArrayList<DisplayObject> getChildren() {
        return this.children;
    }
    

    public boolean contains(DisplayObject DO) {
        return this.contains(DO);
    }


    @Override
    public void update(ArrayList<Integer> pressedKeys){
        super.update(pressedKeys);

        for(DisplayObject c : this.children){
            c.update(pressedKeys);
        }
    }


    @Override
    public void draw(Graphics g){
        super.draw(g);

        Graphics2D g2d = (Graphics2D) g;

        applyTransformations(g2d);

        for(DisplayObject c : this.children) {
            c.draw(g);
        }

        reverseTransformations(g2d);


    }




}
