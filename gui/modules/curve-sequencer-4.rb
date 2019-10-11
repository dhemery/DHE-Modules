
steps = 4
width_hp = 13.0 + steps * 2.25
name "CURVE SEQUENCER #{steps}"

curve_sequencer_file = Pathname('lib/modules/curve-sequencer.rb')
instance_eval(curve_sequencer_file.read, curve_sequencer_file.to_s)
