name 'CUBIC'
hp 5
foreground [180, 100, 30]
background [180, 100, 97]

left = width / 4.0 + 1.0 / 3.0
right = width - left
delta_y = 15.0

top = 20.0

(0..3).each do |row|
  y = top + delta_y * row
  cv_port(x: left, y: y)
  small_knob(x: right, y: y, label: "X<tspan baseline-shift=\"super\">#{3 - row}</tspan>")
end

y = 82.0

small_knob(x: left, y: y, label: 'IN')
small_knob(x: right, y: y, label: 'OUT')

y += delta_y
cv_port(x: left, y: y)
cv_port(x: right, y: y)

y += delta_y
input_port(x: left, y: y)
output_port(x: right, y: y)
