# Integer Based Image Downscaling with Subpixel Accuracy
## Description
Fast image downscaling using integer based arithmetics, described in article:  
https://sigmapi-design.com/media/fast_box_filter.pdf  
Original code uses custom image container. I have modified the code to suite OpenCV Mat, but did not make any further optimization. There is still lot of space for optimization and modifications for differnet types of image containers.
## Required
- C++ compiler (tested on Visual Studio 2017) 
- OpenCV
## Usage
After building, invoke console application:

      capDownScale <srcImage> <scale> <showSrc>
Arguments;
 1. srcImage - source image (mandatory)
 2. scale - scaling number (mandatory) -> image will be downsampled by factor 1/scale.
 3. showSrc - show source image (optional): y|n. Default value is n (do not show original image). 
 
Example:

      capDownScale VIIRS_3Feb2012_lrg.jpg 17     
Note:  
Image is scaled proportionally. To downscale image with separated width/height scales, please modify the source code.        
## Testing
Configuration: Intel Core i5-7400 CPU @ 3.00GHz, 1GB RAM.  
Os: Windows 10.
Program build: Visual Studio 2017, x86 (32-bit), release mode.  
Source image (12000x12000): https://eoimages.gsfc.nasa.gov/images/imagerecords/78000/78314/VIIRS_3Feb2012_lrg.jpg
<table>
 <tr>
  <td>
   Scale factor
   </td>
  <td>
   1/2
   </td>  
  <td>
   1/5
   </td>  
<td>
   1/10
   </td>   
  <td>
   1/15
   </td>    
  <td>
   1/17
   </td>     
  <td>
   1/21
   </td>       
 </tr>
 <tr>
  <td>
   Duration (sec)
  </td>
  <td>
   0.633
  </td>
  <td>
   0.47
  </td>
  <td>
   0.452
  </td>
  <td>
   0.429
  </td>
  <td>
   0.453
  </td>
  <td>
   0.427
  </td>
 </tr>
</table>

## Reference
[1] Petrić S., Fast Box Filter with Subpixel Accuracy. Sigmapi-Design Paperology (2017).
### License
© Siniša Petrić, www.sigmapi-design.com, MIT license.




