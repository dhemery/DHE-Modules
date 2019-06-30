name 'FUZZY LOGIC Z'
hp 9

hue = 240
foreground [hue, 100, 30]
background [hue, 100, 97]

ab_outer_column = hp2mm(1.5)
ab_button_column = hp2mm(3)
ab_inner_column = hp2mm(3.25)
switch_column = hp2mm(4.5)
cd_inner_column = hp2mm(5.75)
cd_button_column = hp2mm(6)
cd_outer_column = hp2mm(7.5)

top = 4
dy = 3

y = hp2mm(top + 0 * dy)
port(x: ab_outer_column, y: y, label: 'A')
button(x: ab_button_column, y: y, label: '¬')
button(x: cd_button_column, y: y, label: '¬')
port(x: cd_outer_column, y: y, label: 'C')

y = hp2mm(top + 0.5 * dy)
polarity_toggle(x: switch_column, y: y, selection: 2)

y = hp2mm(top + 1 * dy)
port(x: ab_outer_column, y: y, label: 'B')
button(x: ab_button_column, y: y, label: '¬')
button(x: cd_button_column, y: y, label: '¬')
port(x: cd_outer_column, y: y, label: 'D')

y = hp2mm(top + 2 * dy)
output_port(x: ab_outer_column, y: y, label: 'AND')
output_port(x: ab_inner_column, y: y, label: '¬')
output_port(x: cd_inner_column, y: y, label: '¬')
output_port(x: cd_outer_column, y: y, label: 'AND')

y = hp2mm(top + 3 * dy)
output_port(x: ab_outer_column, y: y, label: 'OR')
output_port(x: ab_inner_column, y: y, label: '¬')
output_port(x: cd_inner_column, y: y, label: '¬')
output_port(x: cd_outer_column, y: y, label: 'OR')

y = hp2mm(top + 4 * dy)
output_port(x: ab_outer_column, y: y, label: 'XOR')
output_port(x: ab_inner_column, y: y, label: '¬')
output_port(x: cd_inner_column, y: y, label: '¬')
output_port(x: cd_outer_column, y: y, label: 'XOR')

y = hp2mm(top + 5 * dy)
output_port(x: ab_outer_column, y: y, label: 'A &#x22B2; B')
output_port(x: ab_inner_column, y: y, label: '¬')
output_port(x: cd_inner_column, y: y, label: '¬')
output_port(x: cd_outer_column, y: y, label: 'C &#x22B2; D')

y = hp2mm(top + 6 * dy)
output_port(x: ab_outer_column, y: y, label: 'A &#x22B3; B')
output_port(x: ab_inner_column, y: y, label: '¬')
output_port(x: cd_inner_column, y: y, label: '¬')
output_port(x: cd_outer_column, y: y, label: 'C &#x22B3; D')
