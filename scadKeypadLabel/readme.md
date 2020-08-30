### scad drawing for keypad overlay labels

The keypadlabel2d.scad program instructs scad to draw either 4 row by 3 column
or 4 X 4 keypad labels. As given, it's set up for 4 X 4, with dimensions for
a Digitran keypad, with an output example in the .svg file, and a photo showing
one of the labels fitted to a project.

The program parameters can be re-defined to make a similar overlay for a keypad
having different dimensions. There are several changes to make:

`hpitch` - The horizontal distance between keys

`vpitch` - The vertical distance between keys

`cols` - Uncomment to get either 3 or 4 columns

`llkeypos` - the position of the lower left corner of the lower left key [x,y,0]

In the `width` statement after `cols`, specify the overall label width for 3 
and 4 column sizes.

In the body (after `difference( )`):

	- the first `square([width,63],0)` has the overall label height (63 as shown)

	- the second 'square([8.5,7],0)` has the size of hole to cutout for the keys

	- the `font` statement specifies the font to use and its character size.

Finally, an array similar to those shown in the commented definitions needs to
be constructed for the desired labels.




