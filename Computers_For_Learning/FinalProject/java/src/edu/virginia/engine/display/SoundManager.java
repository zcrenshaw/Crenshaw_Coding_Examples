

package edu.virginia.engine.display;

import java.io.*;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import javax.sound.sampled.*;
import javax.swing.*;

// To play sound using Clip, the process need to be alive.
// Hence, we use a Swing application.
public class SoundManager extends TextManager {

    public HashMap soundFX;
    //private Clip music;
    private boolean soundon = true;


    // Constructor
    public SoundManager() {

        this.soundFX = new HashMap();

    }




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
        } catch (UnsupportedAudioFileException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (LineUnavailableException e) {
            e.printStackTrace();
        }
    }


    public void PlaySoundEffect(String id) {
        Clip effect = (Clip)soundFX.get(id);
        if (effect.isRunning()) {
            effect.stop();
        }
        effect.setFramePosition(0);
        effect.start();

    }

    public void PlayTTS(int textstate, String lang) {
        String curr = "text"+textstate+lang;

        Clip effect = (Clip)soundFX.get(curr);

        if (textstate == 0) { //if at start of the game, begin the first tts
            if (effect.isRunning()){
               return;
            }
            effect.setFramePosition(0);
            effect.start();
        } else { //if not at start, must stop previous tts
            String prev = new String();
            switch (textstate) {
                case 14: prev = "text"+(textstate-2)+lang; break;
                case 16: prev = "text"+(textstate-2)+lang; break;
                case 23: prev = "text"+(textstate-2)+lang; break;
                case 26: prev = "text"+(textstate-2)+lang; break;
                default: prev = "text"+(textstate-1)+lang;
            }
            Clip prev_effect = (Clip)soundFX.get(prev);

            if (prev_effect.isRunning()){ //if previous tts is running, stop it and start the new
                prev_effect.stop();
                prev_effect.setFramePosition(0);
                effect.setFramePosition(0);
                effect.start();
            } else { //if prev tts is not running
                if (effect.isRunning()){ // if current tts is running, return
                    return;
                } else {  //if current tts is not running, start it (NOTE: will generate a loop)
                    effect.setFramePosition(0);
                    effect.start();
                }
            }

        }


    }


    public void LoadMusic(String id, String filename) {
        LoadSoundEffect(id,filename);

    }

    public void PlayMusic(String id) {
        Clip music = (Clip)soundFX.get(id);
       if (music.isRunning()) {
           music.stop();
       }

       music.setFramePosition(0);

       music.loop(Clip.LOOP_CONTINUOUSLY);
    }

    public void StopSound(String id) {
        Clip sound = (Clip)soundFX.get(id);
        if (sound.isRunning()) {
            sound.stop();
        }

    }

    public boolean getSoundon() {
        return this.soundon;
    }

    public void setSoundon(boolean b) {
        this.soundon = b;
    }

    public Collection getValues() {return this.soundFX.values();}

    public void toggleMute() {
        Mixer.Info[] infos = AudioSystem.getMixerInfo();
        for (Mixer.Info info: infos) {
            Mixer mixer = AudioSystem.getMixer(info);
            Line [] lines = mixer.getSourceLines();
            for (Line line : lines) {
                BooleanControl bc = (BooleanControl) line.getControl(BooleanControl.Type.MUTE);
                if (bc != null) {
                    bc.setValue(!bc.getValue()); // true to mute the line, false to unmute
                }

            }
        }
        soundon = !soundon;


    }

    public void LoadSFX(){
        LoadMusic("HIP_HOP","summertime.wav");
        LoadMusic("EDM","getaway.wav");
        LoadMusic("POP","starships.wav");
        LoadMusic("ROCK","muse.wav");
        LoadSoundEffect("launched", "launched.wav");
        LoadSoundEffect("added","added.wav");
        LoadSoundEffect("help", "help.wav");
        LoadSoundEffect("settings","settings.wav");
        LoadSoundEffect("reset","reset.wav");
        LoadSoundEffect("max_height_is","max_height_is.wav");

        for (int i = 2; i < 9; i++) {
            LoadSoundEffect("split"+i,"split"+i+".wav");
        }

        for (int i = 0; i < 10; i++) {
            LoadSoundEffect(i+"", i+".wav");
        }
    }

    public void LoadTTS(String lang) {
        for (int i = 0; i < 29; i++) {
            /*switch (i) {
                case 13: break;
                case 15: break;
                case 22: break;
                case 25: break;
                default:
                    String filename = "text" + i + lang;
                    //System.out.println("loading: "+filename);
                    LoadSoundEffect(filename,filename + ".wav");

            }*/

            String filename = "text" + i + lang;
            LoadSoundEffect(filename,filename + ".wav");

        }
    }

    public void ReadHeight(String s, String text) {

        StopSound(text);
        StopSound("launched");
        PlaySoundEffect("max_height_is");
        Sleep(1300);

        char a = s.charAt(0);
        char b = s.charAt(1);
        char c = s.charAt(2);


        PlaySoundEffect(String.valueOf(a));
        Sleep(500);

        PlaySoundEffect(String.valueOf(b));
        Sleep(500);

        PlaySoundEffect(String.valueOf(c));
        Sleep(500);

        if (s.length() == 4) {
            char d = s.charAt(3);
            PlaySoundEffect(String.valueOf(d));
            Sleep(500);
        }



    }

    public void Sleep(int x) {
        try {
            Thread.sleep(x);
        } catch (InterruptedException ex) {
            Thread.currentThread().interrupt();
        }
    }


}