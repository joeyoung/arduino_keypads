### scad drawing for keypad overlay labels

The keypadlabel2d.scad program instructs scad to draw either 4 row by 3 column
or 4 X 4 keypad labels. As given, it's set up for 4 X 4, with dimensions for
a Digitran keypad, with an output example in the .svg file, and a photo showing
one of the labels fitted to a project.

The program parameters can be re-defined to make a similar overlay for a keypad
having different dimensions. There are several changes to make:

 * `hpitch` - The horizontal distance between keys
 * `vpitch` - The vertical distance between keys
 * `cols` - Uncomment one of the lines to get either 3 or 4 columns
 * `llkeypos` - the position of the lower left corner of the lower left key [x,y,0]
 * In the `width` statement after `cols`, specify the overall label width for 3 
   and 4 column sizes.
 * `height` - the overall label height
 * `keyx` - the width of each key hole
 * `keyy` - the height of each key hole
 * In the body (second `translate`) the `font` statement specifies the font to
   use and its character size. There are also constants to adjust the position of the
   labels to centre them over the keys

Finally, an array similar to those shown in the commented definitions needs to
be constructed containing the desired labels.




