---
title: J and S Curves
---

Several of the DHE Modules
apply a curve function
to produce their output.
The curves come in two shapes:

- [J curves](#j-curves)
- [S curves](#j-curves)

Each module's _CURVE_ knob
sets the direction (positive or negative)
and magnitude of the curvature.
Positive curvature creates a J or S shape.
Negative curvature inverts the shape.
Extreme curvature (positive or negative)
creates a curve
so sharp as to be nearly square.

Each curve (J or S, positive or negative)
is produced by
Dino Dini's beautiful
_normalized tunable sigmoid function._
To learn about
this fascinating function,
its mathematical properties,
and how and why Dino Dini invented it,
see [**Normalized Tunable Sigmoid Function**]({{ 'technical/sigmoid/' | relative_url }}).

## J Curves
[_Stage_]({{ '/modules/stage/' | relative_url }})
and
[_Booster Stage_]({{ '/modules/booster-stage/' | relative_url }})
can generate J-shaped envelope stages.

[_Swave_]({{ '/modules/swave/' | relative_url }})
can apply a J-shaped transfer function
to shape its input.

### Positive J Curves

Rotating the _CURVE_ knob
clockwise from the center
_pushes the steep part of the curve to the right._

<table class="curves">
  <tr>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" />
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(30 6.35 6.35)"/>
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(60 6.35 6.35)"/>
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(90 6.35 6.35)"/>
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(120 6.35 6.35)"/>
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(150 6.35 6.35)"/>
      </svg>
    </th>
  </tr>
  <tr>
    <td><img src="jr+0.png" /></td>
    <td><img src="jr+1.png" /></td>
    <td><img src="jr+2.png" /></td>
    <td><img src="jr+3.png" /></td>
    <td><img src="jr+4.png" /></td>
    <td><img src="jr+5.png" /></td>
  </tr>
</table>

**Note:**
Extreme positive curvature
makes the curve almost
square at the end.

### Negative J Curves

Rotating the _CURVE_ knob
counterclockwise from the center
_pushes the steep part of the curve to the left._

<table class="curves">
  <tr>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
          <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" />
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(-30 6.35 6.35)"/>
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(-60 6.35 6.35)"/>
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(-90 6.35 6.35)"/>
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
         <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(-120 6.35 6.35)"/>
       </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(-150 6.35 6.35)"/>
      </svg>
    </th>
  </tr>
  <tr>
    <td><img src="jr+0.png" /></td>
    <td><img src="jr-1.png" /></td>
    <td><img src="jr-2.png" /></td>
    <td><img src="jr-3.png" /></td>
    <td><img src="jr-4.png" /></td>
    <td>—</td>
  </tr>
</table>

**Notes:**
- Each curve
    actually starts at the same voltage.
    But as the curvature becomes more negative,
    the rise is so fast that the Scope
    can't display the initial value.
- With the knob fully counterclockwise,
    the rise is so fast
    that the Scope can't show it at all.
    That's why there's no image.
- Extreme positive curvature
    makes the curve almost
    square at the start.
    Given that there's no image,
    you'll have to trust me
    about this.
    Or try it yourself.

## S Curves

[_Booster Stage_]({{ '/modules/booster-stage/' | relative_url }})
can generate S-shaped envelope stages.

[_Swave_]({{ '/modules/swave/' | relative_url }})
can apply an S-shaped transfer function
to shape its input.

### Positive S Curves

Rotating the _CURVE_ knob
clockwise from the center
_steepens the middle of the S curve._

<table class="curves">
  <tr>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" />
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(30 6.35 6.35)"/>
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(60 6.35 6.35)"/>
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(90 6.35 6.35)"/>
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(120 6.35 6.35)"/>
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(150 6.35 6.35)"/>
      </svg>
    </th>
  </tr>
  <tr>
    <td><img src="sr+0.png" /></td>
    <td><img src="sr+1.png" /></td>
    <td><img src="sr+2.png" /></td>
    <td><img src="sr+3.png" /></td>
    <td><img src="sr+4.png" /></td>
    <td><img src="sr+5.png" /></td>
  </tr>
</table>

**Note:**
Extreme positive curvature
makes the curve almost
square just before and after the middle.

### Negative S Curves

Rotating the _CURVE_ knob
counterclockwise from the center
_flattens the middle of the S curve._

<table class="curves">
    <tr>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
          <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" />
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(-30 6.35 6.35)"/>
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(-60 6.35 6.35)"/>
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(-90 6.35 6.35)"/>
      </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
         <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(-120 6.35 6.35)"/>
       </svg>
    </th>
    <th>
      <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" viewBox="0 0 12.7 12.7">
        <image width="100%" height="100%" x="0" y="0" xlink:href="knob.svg" transform="rotate(-150 6.35 6.35)"/>
      </svg>
    </th>
  </tr>
  <tr>
    <td><img src="sr+0.png" /></td>
    <td><img src="sr-1.png" /></td>
    <td><img src="sr-2.png" /></td>
    <td><img src="sr-3.png" /></td>
    <td><img src="sr-4.png" /></td>
    <td><img src="sr-5.png" /></td>
  </tr>
</table>

**Notes:**
- Extreme negative curvature
    makes the curve almost
    square at both ends.
- Each curve
    actually starts at the same voltage.
    But as the curvature becomes more negative,
    the rise is so fast that the Scope
    can't display the initial value.
- With the knob fully counterclockwise,
    the rise is so fast
    that the Scope can't show the initial rise at all.
