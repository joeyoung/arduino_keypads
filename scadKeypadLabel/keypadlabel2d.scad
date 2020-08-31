//kpadlargerlabel2d.scad  - Digitran keypad overlay generator
// 
// started: November 24(ish) 2019  G. D. (Joe) Young <jyoung@islandnet.com>
//
// revised: Dec 11/19 - runs to separate holes/text
//          Apr 27/20 - for the larger keypads
//
// To get holes, comment the 2nd 'translate' line.
// To get text, comment the first 'translate' line.

// dimensions in mm
// The output s/b exported as .dxf to preserve the mm dimensions

hpitch = 14.0;
vpitch = 13.0;
//hpitch = 12.75;
//vpitch = 12.5;
//thk = 0.1;
llkeypos = [4.3,4.3,0];
//llkeypos = [6,9,0];
cols = 4;
//cols = 3;
width = (cols == 3 ) ? 45.5 : 59.5;
height = 56;
//width = (cols == 3 ) ? 46 : 58;
//height = 63;
keyx = 9;
keyy = 8.5;
//keyx = 8.5;
//keyy = 7;

// labels for clock calibration keypad
keylab = [
     ["C", " ", " ", " "],
     [" ", " ", " ", " "],
     [".", " ", " ", " "],
     ["CAL", "SCALE", "ALM", "TIM"]
    ];
// labels for fox controller eeloader utility
//keylab = [
//     ["_!\x22", "stu", "jkl", "abc"],
//     ["0./", "vwx", "mno", "def"],
//     ["ENT", "yz{", "pqr", "ghi"],
//     ["-->", "<--", "-./", "SHFT"]
//    ];
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
    square([width,height],0);
    for( ix=[0:1:cols-1] ) {
      for( iy=[0:1:3] ){
        translate(llkeypos+[ix*hpitch,iy*vpitch,0])square([keyx,keyy],0);
//        translate(llkeypos+[ix*hpitch+4.25,iy*vpitch+8,0])text(keylab[ix][iy],font="Source Code Pro:style=Light",size=3.5,halign="center");
      };
  };
};

