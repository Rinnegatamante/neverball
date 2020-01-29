package com.drodin.neverball;

import android.content.res.AssetManager;
import android.os.Bundle;

import org.libsdl.app.SDLActivity;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class MySDLActivity extends SDLActivity {
    private AssetManager assetManager;
    public String baseDir;

    public boolean copyAssetFile(String fromAssetPath) {
        try {
            InputStream in = assetManager.open("data/" + fromAssetPath);
            File toFile = new File(baseDir + "/data/" + fromAssetPath);

            if (toFile.exists() && toFile.length() == in.available()) //not for big files
                return true;

            toFile.getParentFile().mkdirs();
            OutputStream out = new FileOutputStream(toFile);
            copyFile(in, out);
            in.close();
            out.flush();
            out.close();
            return true;
        } catch(Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    public boolean copyAsset(String fromAssetPath) {
        if (fromAssetPath == "")
            return false;

        try {
            String[] files = assetManager.list("data/" + fromAssetPath);
            if (files.length == 0)
                copyAssetFile(fromAssetPath);
            else {
                for (String file : files)
                    copyAsset(fromAssetPath + "/" + file);
            }
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    private void copyFile(InputStream in, OutputStream out) throws IOException {
        byte[] buffer = new byte[1024];
        int read;
        while((read = in.read(buffer)) != -1){
            out.write(buffer, 0, read);
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        assetManager = getApplication().getResources().getAssets();

        File baseFileDir = getApplicationContext().getFilesDir();
        baseFileDir.mkdirs();
        baseDir = baseFileDir.getAbsolutePath();

        super.onCreate(savedInstanceState);
    }

    @Override
    protected String[] getLibraries() {
        return new String[] {
                "never" + BuildConfig.FLAVOR
        };
    }
}
