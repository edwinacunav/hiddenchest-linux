class Window_Command
  def draw_item(index, enabled = true)
    c = self.contents
    rect = item_rect(index)
    rect.x += 4
    rect.width -= 8
    @area[index] = [self.x+20, self.y+16+index*WLH, c.width-8, WLH]
    c.clear_rect(rect)
    c.font.color = normal_color
    c.font.color.alpha = enabled ? 255 : 128
    c.draw_text(rect, @commands[index])
  end

  def x=(newx)
    @area.each{|a| a[0] = newx + 20 }
    super(newx)
  end

  def y=(newy)
    @area.size.times{|n| @area[n][1] = newy + 16 + n * WLH }
    super(newy)
  end
end

class Scene_Title
  def create_title_graphic
    @sprite = Sprite.new
    @sprite.bitmap = Cache.system("Title")
    @face = Sprite.new
    @face.x = 16
    @face.y = 32
    @face.bitmap = Bitmap.new(96, 96)
    b = Cache.face('Actor1')
    rect = Rect.new(0, 0, 96, 96)
    @face.bitmap.blt(0, 0, b, rect)
    b.dispose
  end

  def create_command_window
    opt = [Vocab::new_game, Vocab::continue, Vocab::shutdown]
    @command_window = Window_Command.new(172, opt)
    @command_window.x = (Graphics.width - 172) / 2
    @command_window.y = 288
    if @continue_enabled
      @command_window.index = 1
    else
      @command_window.draw_item(1, false)
    end
    @command_window.openness = 0
    @command_window.open
    @cx, @cy, @cw, @ch = @command_window.area[0]
  end

  def terminate
    dispose_command_window
    snapshot_for_background
    dispose_title_graphic
    @face.bitmap.dispose
  end

  def update
    @face.update
    @command_window.update
    if Input.trigger?(Input::C)
      case @command_window.index
      when 0 then command_new_game
      when 1 then command_continue
      when 2 then command_shutdown
      end
    elsif Input.press?(Input::MouseLeft)
      if @command_window.mouse_above?(0)
        @command_window.index = 0
        return command_new_game
      elsif @command_window.mouse_above?(1)
        @command_window.index = 1
        return command_continue
      elsif @command_window.mouse_above?(2)
        @command_window.index = 2
        return command_shutdown
      elsif @face.mouse_above?
        @face.flash(Color.new(255, 255, 255, 120), 2)
        return
      end
    end
  end
end