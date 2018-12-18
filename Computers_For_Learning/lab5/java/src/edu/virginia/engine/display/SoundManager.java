

package edu.virginia.engine.display;

import java.io.*;
import java.util.HashMap;
import javax.sound.sampled.*;
import javax.swing.*;

// To play sound using Clip, the process need to be alive.
// Hence, we use a Swing application.
public class SoundManager extends JFrame {

    private HashMap soundFX;
    //private Clip music;


    // Constructor
    public SoundManager() {
        /*
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setTitle("Test Sound Clip");
        this.setSize(300, 200);
        this.setVisible(true);
        */

        this.soundFX = new HashMap();

/*
        try {
            // Open an audio input stream.
            File soundFile = new File("resources" + File.separator + "happy_mario.wav");
            AudioInputStream audioIn = AudioSystem.getAudioInputStream(soundFile);
            // Get a sound clip resource.
            Clip clip = AudioSystem.getClip();
            // Open audio clip and load samples from the audio input stream.
            clip.open(audioIn);
            clip.loop(Clip.LOOP_CONTINUOUSLY);
        } catch (UnsupportedAudioFileException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (LineUnavailableException e) {
            e.printStackTrace();
        }

        */


    }


    /*
    public static void main(String[] args) {
        SoundManager sounds = new SoundManager();
        sounds.LoadMusic("background","happy_mario.wav");
        sounds.PlayMusic("background");
        System.out.println(sounds.soundFX.size());
        System.out.println(sounds.soundFX.containsKey("music"));

    }
    */

    public void LoadSoundEffect(String id, String filename) {
        try {
            // Open an audio input stream.
            File soundFile = new File("resources" + File.separator + filename);
            AudioInputStream audioIn = AudioSystem.getAudioInputStream(soundFile);
            // Get a sound clip resource.
            Clip clip = AudioSystem.getClip();
            // Open audio clip and load samples from the audio input stream.
            clip.open(audioIn);
            this.soundFX.put(id,clip);
           // System.out.println("Loaded: " + id + " successful?: " + soundFX.containsKey(id));
        } catch (UnsupportedAudioFileException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (LineUnavailableException e) {
            e.printStackTrace();
        }
    }


    public void PlaySoundEffect(String id) {
        //System.out.println("Playing: "+id);
       // System.out.println("SoundFX contains "+ id+ " "+ soundFX.containsKey(id));
        Clip effect = (Clip)soundFX.get(id);
      //  System.out.println("Found: "+effect);
        if (effect.isRunning()) {
            effect.stop();
        }
        effect.setFramePosition(0);
        effect.start();

    }


    public void LoadMusic(String id, String filename) {
        LoadSoundEffect(id,"happy_mario.wav");
      //  System.out.println("Loaded music id: " + id+  " successful?: " +soundFX.containsKey(id));

    }

    public void PlayMusic(String id) {
        Clip music = (Clip)soundFX.get(id);
       if (music.isRunning()) {
       //    System.out.println("Music is running\n");
           music.stop();
       }
      //  System.out.println("Playing music\n");
       music.setFramePosition(0);

       //music.start();
       music.loop(Clip.LOOP_CONTINUOUSLY);
     //  System.out.println("HELLO");
    }

    public void StopSound(String id) {
      //  System.out.println("Stopping: "+id);
       // System.out.println(soundFX.containsKey(id));
        Clip sound = (Clip)soundFX.get(id);
        if (sound.isRunning()) {
         //   System.out.println(id+ " is running\n");
            sound.stop();
          //  System.out.println("Stopping it now");
        }
      //  System.out.println("Couldn't stop "+id);

    }



}