require_relative 'booster_stage'

module DHE
  class Modules
    def self.all
      [
          BoosterStage.new
      ]
    end
  end
end
