# = Very primative lander game
# 
# For now this is going to be 2D and use SDL. 
# Kinda crappy because the code is anything but optimised for speed and 
# I want to later port this to some OpenGL bindings and make it 3D, when I 
# get a day or two to pick up some basic OpenGL
#
# Was for a weekend coding event. Kinda wacked up quicker though
# author:: Jason Earl <jason@hybd.net>
# date:: 07/10/07

gem_original_require 'RUDL' rescue require 'RUDL' # Get round GEM's conflict
require 'singleton'
require 'yaml'

include Math, RUDL
PI2           = 2 * PI
SCREEN_WIDTH  = 800
SCREEN_HEIGHT = 600
BLACK         = [ 0, 0, 0, 255  ]
WHITE         = [ 255, 255, 255 ]
YELLOW        = [ 255, 255, 0   ]
RED           = [ 255, 0, 0     ]

# Util angle stuff
class Numeric
  DEGRAD = 180 / PI
  
  def deg2rad
    self / DEGRAD
  end
  
  def rad2deg
    self * DEGRAD
  end
end

module SharedCrap
  private

  def smooth_interpolate(start, finish, scale)
    cubic_scale = (1 - cos(scale * PI)) / 2
    linear_interpolate start, finish, cubic_scale
  end
  
  def linear_interpolate(start, finish, scale)
    (start * (1 - scale)) + (finish * (scale))
  end
  
  def blend_colour(start, finish, scale)
    (0...start.size).map { |channel| linear_interpolate start[channel], finish[channel], scale }
  end
  
  def wait_for_key
    loop do
      EventQueue.pump
      return unless Key.pressed?.empty?
      sleep 0.1
    end
  end
  
  def centre_text(text, y, colour = WHITE, image = surface)
    return if text.empty?
    banner = font.render text, true, colour
    image.blit banner, [ (SCREEN_WIDTH - banner.w) / 2, y ]
  end
end

class Lander

  include SharedCrap

  attr_reader \
    :angle, 
    :acceleration, 
    :x, 
    :y, 
    :font,
    :image,
    :active_image,
    :collision_map,
    :scores, 
    :gravity, 
    :rotation_speed, 
    :thrust_speed, 
    :thrust_limit, 
    :thrust_decay,
    :total_power,
    :power
  
  # Some basic specs that define the lander behaviour
  DEFAULT_SPECS = { 
    :rotation_speed => 1.deg2rad, 
    :thrust_speed   => 0.1, 
    :thrust_limit   => 6, 
    :thrust_decay   => 0.025,  
    :gravity        => 3,
    :power          => 25 } 

  WIDTH = 40
  
  def initialize(specifications = {})
    change_specs DEFAULT_SPECS.update(specifications)
    @scores = []
    @image = Surface.load_new 'ship.png'
    @font  = TrueTypeFont.new 'akbar.ttf', 18
    image.set_colorkey BLACK 
    reset_position!
  end

  # For now a rotating sprite will do the job. Crap I know
  def draw(surface)
    surface.blit active_image, [ sprite_x, y ]
    surface.fill WHITE, [ 10, 10, (power / @total_power) * 100,20]
    banner = font.render "Thrust Remaining", true, YELLOW
    surface.blit banner, [ 10, 30 ]
    banner = font.render "Total Score: #{total_score}", true, YELLOW
    surface.blit banner, [ SCREEN_WIDTH - 10 - banner.w, 30 ]
  end
  
  def increase_thrust
    return if power < 0
    @acceleration += thrust_speed unless acceleration > thrust_limit
    @power -= thrust_speed
  end
  
  def rotate_left
    @angle -= rotation_speed
    normalize_angle 
    update_image
  end
  
  def rotate_right
    @angle += rotation_speed
    normalize_angle
    update_image
  end
  
  def sprite_x
    x - (width / 2)
  end
  
  def update_position
    @old_y = y
    # Apply some very primate gravity 
    @y += gravity
    # Account for thrust
    @x -= acceleration * cos(angle)
    @y += acceleration * sin(angle)
    # Decay thrust / acceleration
    @acceleration -= thrust_decay
    @acceleration  = 0 if acceleration < 0    
  end
  
  # Work out new score based on landing parameters
  def completed(start_time, acceleration_delta, bumpiness)
    time = Time.now - start_time
    bumpiness = 0.1 if bumpiness == 0 # Avoid DIV0 error
    skill = (3.0 / time) * (1.0 / acceleration_delta) * (1.0 / bumpiness) # A very crude way to judge skill / assign score
    scores << (100 * [ skill, 10 ].min).to_i # Clamp score 
    scores.last
  end
  
  # Add up all scores
  def total_score
    scores.compact.inject(0) { |total, score| total += score }
  end
  
  # Move to initial position
  def reset_position!
    @y = 100
    @x = 400 # Needs to be in centre, probably
    @angle = -PI / 2
    @acceleration = 0
    update_image
  end
  
  # Specs of lander define it's behaviour in the world
  def change_specs(specifications = {})
    @rotation_speed = specifications[:rotation_speed] if specifications[:rotation_speed]
    @thrust_speed   = specifications[:thrust_speed]   if specifications[:thrust_speed]
    @thrust_limit   = specifications[:thrust_limit]   if specifications[:thrust_limit]
    @thrust_decay   = specifications[:thrust_decay]   if specifications[:thrust_decay]
    @gravity        = specifications[:gravity]        if specifications[:gravity]
    @power          = specifications[:power]          if specifications[:power]
    @total_power    = power
  end
  
  def draw_crash(surface)
    for i in 1..10
      update_position
      surface.filled_circle [ x + active_image.w / 2, y + active_image.h / 2 ], WIDTH * 0.7 - i * 1.5, blend_colour(RED + [ 60 ], YELLOW + [ 50 ], i / 10.0)
    end
  end
  
  def speed
    @old_y - y
  end
  
  def width
    image.w
  end
  
  private
  
  def update_image
    @active_image  = image.rotozoom angle.rad2deg + 90, 1, true
    @collision_map = CollisionMap.new active_image
  end
  
  def normalize_angle
    temp = angle % PI2
    @angle = temp - PI2 if temp >  PI
    @angle = temp + PI2 if temp < -PI
  end
end

class Terrain
  
  attr_reader :width, :height, :points, :buffer, :collision_map
  
  include SharedCrap
  
  # Colours
  TOP     = [ 160, 140, 100 ]
  BOTTOM  = [ 255, 230, 140 ]
  
  def initialize(width, height)
    @width, @height = width, height
    @buffer = Surface.new [ width, height ]
    buffer.set_colorkey BLACK
  end
  
  def generate
    @points = []
    target_y, landable = false, false
    x, y = 0, rand * height / 2
    while x <= width
      # Define target
      distance = Lander::WIDTH + rand * 50
      if distance < Lander::WIDTH * 1.5 and not landable and target_y then 
        target_y = y
        landable = true
      else
        target_y = (rand * height / 4) * 2
      end
      # generate points through interpolation
      for offset in 0..distance
        points[x + offset] = height / 2 + smooth_interpolate(y, target_y, offset / distance)
      end
      # Continue where we left off
      y  = target_y
      x += distance
    end
    generate if not landable # Very inefficient brute force / recursive way to make somewhere landable
    draw_to_buffer
    @collision_map = CollisionMap.new buffer
    add_stars
    self
  end
  
  def draw(surface)
    surface.blit buffer, [0, 0]
  end
  
  def collides_with?(lander)
    collision_map.collides_with [ 0, 0 ], lander.collision_map, [ lander.sprite_x, lander.y ]
  end
  
  def bumpiness_at(x)
    range = range_at x
    range.max - range.min
  end
  
  def range_at(x)
    w = Lander::WIDTH / 2
    points[x - w/2,w]
  end
  
  private 
  
  def draw_to_buffer
    buffer.fill BLACK
    for x in 0...width
      start = points[x].to_i
      for y in start..height
        scale = (y - start) / (height - start).to_f
        # Yes, this is very slow. Should really use another technique to aviod Ruby -> C context switch
        buffer[x, y] = blend_colour TOP, BOTTOM, scale
      end
    end
  end
  
  def add_stars
    200.times do 
      x, y = rand * SCREEN_WIDTH, rand * SCREEN_HEIGHT
      buffer[x, y] = WHITE + [ rand * 255 ] if buffer[x, y] == BLACK
    end
  end
end

class Environment

  attr_reader \
    :terrain, 
    :lander, 
    :start_time, 
    :speed_threshold, 
    :angle_threshold,
    :bump_threshold
  
  def button_map
    {
      :thrust => RUDL::Constant::K_SPACE,
      :left   => RUDL::Constant::K_LEFT,
      :right  => RUDL::Constant::K_RIGHT,
      :quit   => RUDL::Constant::K_ESCAPE,
    }
  end
  
  def initialize
    # Set limits
    @bump_threshold  = 8.0
    @angle_threshold = 15
    @speed_threshold = 1.3
    @terrain = Terrain.new SCREEN_WIDTH, SCREEN_HEIGHT
    @lander  = Lander.new
    reset
  end

  def reset
    @start_time = Time.now
    lander.reset_position!
    terrain.generate
  end

  def process
    # Draw etc
    sleep 1.0 / 50 # Clamp frame rate 
    draw
    lander.update_position
    # Deal with collision or landing
    return :off_screen if off_screen?
    if terrain.collides_with? lander
      bumpiness = terrain.bumpiness_at lander.x
      speed_magnitude = lander.speed.abs
      return :not_level unless angle_range.include? lander.angle
      return :too_fast  unless speed_magnitude <= speed_threshold
      return :too_bumpy unless bumpiness <= bump_threshold
      lander.completed start_time, speed_magnitude, bumpiness
      return :landed
    end
    handle_input
  end
  
  # Make each attempt progessively harder
  def next
    # Make things harder
    @bump_threshold  /= 1.2
    @angle_threshold /= 1.2
    @speed_threshold /= 1.2 
    # Stop it from being impossible
    @bump_threshold = 1    if bump_threshold  < 1
    @angle_threshold = 2   if angle_threshold < 2
    @speed_threshold = 0.3 if speed_threshold < 0.3
    lander.change_specs :power => lander.total_power / 1.2, :gravity => lander.gravity * 1.1
    # Reset surroundings
    terrain.generate
    lander.reset_position!
  end
  
  def draw
    game.surface.flip
    game.surface.fill BLACK
    terrain.draw game.surface
    lander.draw  game.surface    
  end
  
  def handle_input
    EventQueue.pump
    key_presses = Key.pressed?
    lander.increase_thrust  if key_presses[button_map[:thrust]]                        
    lander.rotate_left      if key_presses[button_map[:right]]
    lander.rotate_right     if key_presses[button_map[:left]]
    exit                    if key_presses[button_map[:quit]]
  end
  
  private
  
  def off_screen?
    lander.x < 0 or lander.x - Lander::WIDTH > SCREEN_WIDTH or lander.y < 0
  end
  
  def angle_range
    base = -PI/2
    (base - angle_threshold.deg2rad)..(base + angle_threshold.deg2rad)
  end

  def game
    Game.instance
  end
end

class Game
   
  attr_reader :surface, :environment, :font
  
  include Singleton, SharedCrap
   
  def initialize
    display_mode = [ [ SCREEN_WIDTH, SCREEN_HEIGHT ] ]
    raise 'Can not set display mode' unless DisplaySurface.mode_ok? *display_mode
    @surface = DisplaySurface.new(*display_mode).set_caption('Blarg, it`s Lander')
    @environment = Environment.new
    @font = TrueTypeFont.new 'akbar.ttf', 40
  end
  
  def run
    5.times do 
      status = nil
      loop do
        status = environment.process
        break if status
      end
      environment.lander.draw_crash surface unless :landed == status
      message = case status
        when :landed      then "Well Done, you scored : #{environment.lander.scores.last}"
        when :not_level   then "Bad luck, your ship was not level"
        when :too_fast    then "You've crashed!"
        when :too_bumpy   then "The surface is too bumpy"
        when :off_screen  then "Meh. You've flow too far away"
      end
      banner = font.render message, true, WHITE
      surface.blit banner, [ (SCREEN_WIDTH - banner.w) / 2, 100 ]
      surface.update
      environment.next
      wait_for_key
    end
  end
  
  def intro_screen
    surface.fill BLACK
    centre_text "Lunar Lander", 36
    instructions = [
      "A slap-up game writen in a day or so.",
      "",
      "Use space to thrust",
      "Use left and right arrows to tilt",
      "",
      "Try to land your ship on flat land",
      "You must land slowly and level...",
      "to avoid self destruction",
      "",
      "Each attempt will get harder to land!",
      "You have 5 attempts" ]
    y = 70
    for line in instructions
      y += 42
      next if line.empty?
      centre_text line, y, YELLOW
    end
    surface.update
    wait_for_key
  end
  
  def over
    surface.fill BLACK
    centre_text "Game Over. KTHXBYE", 36
    centre_text "You scored a total of #{environment.lander.total_score}", 100, YELLOW
    surface.update
    sleep 0.5
    wait_for_key
  end
  
end

if __FILE__ == $0
  game = Game.instance
  game.intro_screen
  game.run
  game.over
end