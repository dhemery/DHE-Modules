name 'GATOR'
hp 9

hue = 240
foreground [hue, 100, 30]
background [hue, 100, 97]

top = hp2mm(3.5)
dy = hp2mm(1.75)

left = hp2mm(1.5)
lc = hp2mm(3.25)
rc = hp2mm(5.75)
right = hp2mm(7.5)

(1..8).each do |r|
  row = top + (r - 1) * dy
  port(x: left, y: row)
  button(x: lc, y: row)
  button(x: rc, y: row)
  port(x: right, y: row)
end

row = top + 8.75 * dy
output_port(x: left, y: row, label: 'AND')
output_port(x: lc, y: row, label: 'NAND')
output_port(x: rc, y: row, label: 'OR')
output_port(x: right, y: row, label: 'NOR')

row += hp2mm(2.75)
output_port(x: left, y: row, label: 'EVEN')
output_port(x: lc, y: row, label: 'ODD')
output_port(x: rc, y: row, label: 'XOR')
output_port(x: right, y: row, label: 'XNOR')
