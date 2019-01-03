require_relative 'booster_stage'
require_relative 'cubic'
require_relative 'func'
require_relative 'func_6'
require_relative 'hostage'

module DHE
  class Modules
    def self.all
      [
          BoosterStage.new,
          Cubic.new,
          Func.new,
          Func6.new,
          Hostage.new,
      ]
    end
  end
end
