name 'FLOGIC'
hp 9

hue = 240
foreground [hue, 100, 30]
background [hue, 100, 97]

left = hp2mm(1.5)
lc = hp2mm(3.25)
rc = hp2mm(5.75)
right = hp2mm(7.5)

top = 4
dy = 3.5

y = hp2mm(top + 0 * dy)
port(x: left, y: y, label: 'A')
button(x: lc, y: y, label: '¬')
button(x: rc, y: y, label: '¬')
port(x: right, y: y, label: 'B')

y = hp2mm(top + 1 * dy)
output_port(x: left, y: y, label: 'A ∧ B')
output_port(x: lc, y: y, label: '¬')
output_port(x: rc, y: y, label: '¬')
output_port(x: right, y: y, label: 'A ⋂ B')

y = hp2mm(top + 2 * dy)
output_port(x: left, y: y, label: 'A ∨ B')
output_port(x: lc, y: y, label: '¬')
output_port(x: rc, y: y, label: '¬')
output_port(x: right, y: y, label: 'A ∪ B')

y = hp2mm(top + 3 * dy)
output_port(x: left, y: y, label: 'A ⊻ B')
output_port(x: lc, y: y, label: '¬')
output_port(x: rc, y: y, label: '¬')
output_port(x: right, y: y, label: 'A ⊕ B')

y = hp2mm(top + 4 * dy)
output_port(x: left, y: y, label: 'A ⇥ B')
output_port(x: lc, y: y, label: '¬')
output_port(x: rc, y: y, label: '¬')
output_port(x: right, y: y, label: 'A ⊃ B')

y = hp2mm(top + 5 * dy)
output_port(x: left, y: y, label: 'A ⇤ B')
output_port(x: lc, y: y, label: '¬')
output_port(x: rc, y: y, label: '¬')
output_port(x: right, y: y, label: 'A ⊂ B')
