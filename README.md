# IB_downscale
## Description
Fast image downscaling using integer based arithmetics, described in article:  
https://sigmapi-design.com/media/fast_box_filter.pdf
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
## Testing
Configuration: Intel Core i5-7400 CPU @ 3.00GHz, 1GB RAM.  
Program build: Visual Studio 2017, x32, debug mode.  
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
   14.97
  </td>
  <td>
   11.171
  </td>
  <td>
   9.452
  </td>
  <td>
   9.716
  </td>
  <td>
   8.947
  </td>
  <td>
   8.834
  </td>
 </tr>
</table>

### License
© Siniša Petrić, www.sigmapi-design.com, MIT license.

## Reference
[1] Petrić S., FastBox Filter with Subpixel Accuracy. Sigmapi-Design Paperology (2017).



