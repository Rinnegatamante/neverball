package com.drodin.neverball;

import org.libsdl.app.SDLActivity;

public class MySDLActivity extends SDLActivity {
    @Override
    protected String[] getLibraries() {
        return new String[] {
                "never" + BuildConfig.FLAVOR
        };
    }
}
