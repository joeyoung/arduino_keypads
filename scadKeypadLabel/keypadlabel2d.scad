//keypadlabel2d.scad  - Digitran keypad overlay generator
// 
// started: November 24(ish) 2019  G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised: Dec 11/19 - runs to separate holes/text
//
// To get holes, comment the 2nd 'translate' line.
// To get text, comment the first 'translate' line.

// dimensions in mm
// The output s/b exported as .dxf to preserve the mm dimensions

hpitch = 12.75;
vpitch = 12.5;
//thk = 0.1;
llkeypos = [6,9,0];
cols = 4;
//cols = 3;
width = (cols == 3 ) ? 46 : 58;

// labels for clock calibration keypad
//keylab = [
//     ["C", " ", " ", " "],
//     [" ", " ", " ", " "],
//     [".", " ", " ", " "],
//     ["CAL", "SCALE", "ALM", "TIM"]
//    ];
// labels for fox controller eeloader utility
keylab = [
     ["_!\x22", "stu", "jkl", "abc"],
     ["0./", "vwx", "mno", "def"],
     ["ENT", "yz{", "pqr", "ghi"],
     ["-->", "<--", "-./", "SHFT"]
    ];
// labels for calculator left 16-key keypad
//keylab = [
//     ["CLR", " ", " ", " "],
//     [" ", " ", " ", " "],
//     [".", " ", " ", " "],
//     ["=", "*", "-", "+"]
//    ];
// labels for calculator right 12-key keypad
//keylab = [
//     ["CLR", "C", "B", "A"],
//     ["0", "F", "E", "D"],
//     [" ", "SQRT", "MOD", "/"],
//     ["=", "*", "-", "+"]
//    ];

difference( ){
    square([width,63],0);
    for( ix=[0:1:cols-1] ) {
      for( iy=[0:1:3] ){
        translate(llkeypos+[ix*hpitch,iy*vpitch,0])square([8.5,7],0);
        translate(llkeypos+[ix*hpitch+4.25,iy*vpitch+8,0])text(keylab[ix][iy],font="Source Code Pro:style=Light",size=3.5,halign="center");
      };
  };
};

