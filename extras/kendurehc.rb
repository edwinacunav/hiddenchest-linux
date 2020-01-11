# * KEndure HiddenChest (XP + VX)
#   Scripter : Kyonides Arkanthes

# This scriptlet allows you to set Endure like states in your games!
# If your hero reaches a point where his HP go below n points or percent, it
# will not let it happen, keeping him alive!

# Of course, if the state has a set number of turns before it vanishes, he may
# inevitably die the next turn...

# Warning! These states will not reduce a hero's MAXHP as the DB does.

module KEndure
   # Type Options: nil - plain HP, true - HP percent, :curse - cannot heal!
  # ID => [Integer, Type]
  STATE = { 19 => [1, nil], 20 => [25, true], 21 => [1, :curse] }
  STATE_IDS = STATE.keys.sort
end

class Game_Battler
  def hp=(nhp)
    if @states.size > 0
      KEndure::STATE_IDS.each do |sid|
        next unless @states.include?(sid)
        n, percent = KEndure::STATE_IDS[sid]
        n *= maxhp / 100 if percent == true
        return @hp = n if (percent == :curse and nhp > 0) or @hp + nhp < n
      end
    end
    @hp = nhp.clamp(0, maxhp)
    @states.each do |i|
      next unless $data_states[i].zero_hp
      self.dead? ? add_state(i) : remove_state(i)
    end
  end

  alias :kyon_endure_gm_battler_as :add_state
  def add_state(sid)
    return if (KEndure::STATE_IDS & [sid]).any?
    kyon_endure_gm_battler_as(sid)
  end
end