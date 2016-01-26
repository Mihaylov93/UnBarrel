# UnBarrel
Simple implementation of the barrel distortion correction for JPEG

~~Can also correct pincushion distortion~~

## Introduction:
##### What is distortion?

In geometric optics distortion is a deviation from rectilinear projection, a projection in which
straight lines in a scene remain straight in an image. It is a form of optical aberration.

##### Barrel distortion

In barrel distortion, image magnification decreases with distance from the optical axis. The apparent
effect is that of an image which has been mapped around a sphere (or barrel).Fisheye lenses, which
take hemispherical views, utilize this type of distortion as a way to map an infinitely wide object
plane into a finite image area. In a zoom lens barrel distortion appears in the middle of the lens's
focal length range and is worst at the wide-angle end of the range.

##### Determining lens correction
**a**, **b**, **c** and **FoV** are physical properties of a lens/camera-combination at a given focus distance. If you always shoot at the same focus setting, f.e. infinity or the hyperfocal distance, then you can safely reuse the parameters. At different focus settings, FoV will change noticeably.

There are a number of ways to determine the a, b, c and fov parameters to calibrate a particular lens/camera combination.For more information http://hugin.sourceforge.net/docs/manual/Lens_correction_model.html#Determine_lens_correction

Or find your lens parameters from a database like http://lensfun.sourceforge.net/

#####  a, b & c parameters:

The lens distortion a, b and c parameters correspond to a third degree polynomial describing radial lens distortion:
![alt text](http://hugin.sourceforge.net/docs/manual/d4466e5ff97cd6bbdddc514f3a28fb88.png "Formula")

where rDest and rSrc refer to the normalized radius of an image pixel. The center point of this radius is where the optical axis hits the image - normally the image center. Normalized means here that the largest circle that completely fits into an image is said to have radius=1.0 . (In other words, radius=1.0 is half the smaller side of the image.) A perfect lens would have **a=b=c=0.0** and d=1.0 which resolves into **rDest == rSrc**

Usual values for **a**, **b** and **c** are below 1.0, in most cases below 0.01. Too high values suggest that you chose a wrong lens type, f.e. fisheye instead of rectilinear or vice versa. This refers to the absolute values of course since a, b and c can be positive or negative (f.e. both 4.5 and -4.5 are considered too high values).

##### Example:
Photo taken with the SJ4000 Camera

![alt text](https://cloud.githubusercontent.com/assets/8819640/12583355/9ae331b2-c442-11e5-8c9a-d38329e87922.JPG "OriginalImg")

Parameters a = 0 ; b 0.1; c = 0.00001; s = 0.799




![alt text](https://cloud.githubusercontent.com/assets/8819640/12583366/b41bf790-c442-11e5-804d-7a802b0d32ad.jpg "OutputImg")



### Useful links:

https://4pi.org/downloads/  **abc.xls**  
  > An Excel spreadsheet, which allows the user to distort a regular grid using the lens parameters a, b and c
  
http://hugin.sourceforge.net/docs/manual/Lens_correction_model.html



http://lensfun.sourceforge.net/ - A huge database of lenses with their parameters.
