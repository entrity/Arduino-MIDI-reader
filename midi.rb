#! /usr/bin/env ruby
#
# This script looks for file ./sounds/*.mid, parses all of them and creates
# ./sounds/*.c. Each of the .c files holds code for creating an MjMidi::Song.
#
# This file holds a modified example, reader2text.rb, from the examples
# included with the midilib gem
# - Markham

require 'rubygems'
require 'midilib'

require 'midilib/sequence'
require 'midilib/io/midifile'

DEFAULT_MIDI_TEST_FILE = 'data-short.mid'
$offset = 55

def run
  @seq = MIDI::Sequence.new()

  # Specify what class to use when reading the MIDI file.
  @seq.reader_class = TextTranslator

  Dir['sounds/*.mid'].each do |input_file|
    output_file = input_file.sub(/.mid$/,'.c')
    $out = []
    puts "writing to #{output_file}"
    File::open(input_file,'rb') do |input|
      @seq.read(input)
    end
    File::open(output_file,"wb"){|output|
      song_name = File::basename(input_file,'.mid').gsub(/-/,'_')
      array_name = "#{song_name}_midiEvents"
      output << "MjMidi::event #{array_name}[] = {"
      output << $out.map{|x| "\n\t{#{x}}" }.join(",")
      output << "\n};"
      output.puts
      output.puts
      output.puts "MjMidi::Song #{song_name}(&#{array_name}[0], #{$out.length});"
    }
  end
end

class TextTranslator < MIDI::IO::MIDIFile

  def initialize(seq, proc = nil)
    super()
    @seq = seq
    @track = nil
    @update_block = block_given?() ? Proc.new() : proc
  end

  # Generate a unique number for a channel/note combination. This is used
  # to remember pending note on events.
  def note_hash(chan, note)
    return (chan << 8) + note
  end

  # Print a delta time.
  def pdelta
    print "#{@curr_ticks}: "
    $offset = @curr_ticks
  end

  # The remaining methods are overrides of methods in MIDI::IO::MIDIFile.

  def header(format, ntrks, division)
    puts "header: format = #{format}, ntrks = #{ntrks}," +
      " division = #{division}"

    @ntrks = ntrks
    @update_block.call(nil, @ntrks, 0) if @update_block
  end

  def start_track()
    pdelta()
    puts "track start"

    @pending = []
    @chan_mask = 0
  end

  def end_track()
    pdelta()
    puts "track end; chans used bitmask = #{@chan_mask}"
    # Write message for any pending note on messages
    @pending.each_with_index do |num, chan|
      puts "pending note off missing for chan #{num >> 8}," +
      " note #{num & 0xff}" if note_obj
    end
    @pending = nil

    # call update block
    @update_block.call(@track, @ntrks, @seq.tracks.length) if @update_block
  end

  def note_on(chan, note, vel)
    pdelta()
    if vel == 0
      print "(note on, vel 0) "
      note_off(chan, note, 64)
      return
    end

    puts "note on chan #{chan}, note #{note}, vel #{vel}"
    $out << "%3d,MIDI_NOTE_ON ,%3d" % [$offset, note]
    @pending << note_hash(chan, note)
    track_uses_channel(chan)
  end

  def note_off(chan, note, vel)
    pdelta()
    # Find note on, create note off, connect the two, and remove
    # note on from pending list.
    pnum = note_hash(chan, note)
    @pending.each_with_index do |num, i|
      if pnum == num
        puts "note off chan #{chan}, note #{note}, vel #{vel}"
        $out << "%3d,MIDI_NOTE_OFF,%3d" % [$offset, note]
        @pending.delete_at(i)
        return
      end
    end
    puts "note off with no earlier note on (ch #{chan}, note" +
      " #{note}, vel #{vel})"
  end

  def pressure(chan, note, press)
    pdelta()
    puts "pressure chan #{chan}, note #{note}, press #{press}"
    track_uses_channel(chan)
  end

  def controller(chan, control, value)
    pdelta()
    puts "controller chan #{chan}, control #{control}, value #{value}"
    track_uses_channel(chan)
  end

  def pitch_bend(chan, msb, lsb)
    pdelta()
    puts "pitch bend chan #{chan}, msb #{msb}, lsb #{lsb}"
    track_uses_channel(chan)
  end

  def program(chan, program)
    pdelta()
    puts "program chan #{chan}, program #{program}"
    track_uses_channel(chan)
  end

  def chan_pressure(chan, press)
    pdelta()
    puts "chan press chan #{chan}, press #{press}"
    track_uses_channel(chan)
  end

  def sysex(msg)
    pdelta()
    puts "sysex size #{msg.length}"
  end

  def meta_misc(type, msg)
    pdelta()
    puts "meta misc type #{type}, length #{msg.length}"
  end

  def sequencer_specific(type, msg)
    pdelta()
    puts "sequencer specific type #{type}, msg #{msg.length}"
  end

  def sequence_number(num)
    pdelta()
    puts "sequence number #{num}"
  end

  def text(type, msg)
    pdelta()
    msg = MIDI::MetaEvent.bytes_as_str(msg)
    case type
    when MIDI::META_SEQ_NAME
      puts "seq or track name #{msg}"
    when MIDI::META_INSTRUMENT
      puts "instrument name #{msg}"
    when MIDI::META_MARKER
      puts "marker #{msg}"
    else
      puts "text = #{msg}, type = #{type}"
    end
  end

  def eot()
    pdelta()
    puts "end of track event"
  end

  def time_signature(numer, denom, clocks, qnotes)
    pdelta()
    puts "time sig numer #{numer}, denom #{denom}, clocks #{clocks}," +
      " qnotes #{qnotes}"
  end

  def smpte(hour, min, sec, frame, fract)
    pdelta()
    puts "smpte #{hour}:#{min}.#{sec}, frame #{frame}, fract #{fract}"
  end

  def tempo(microsecs)
    pdelta()
    bpm = 1.0 / microsecs # quarter notes per microsecond
    bpm *= 1000000.0  # quarter notes per second
    bpm *= 60.0   # quarter notes per minute
    puts "tempo microsecs pqn = #{microsecs} (#{bpm} bpm)"
  end

  def key_signature(sharpflat, is_minor)
    pdelta()
    puts "key sig sharpflat #{sharpflat}, is_minor #{is_minor}"
  end

  def arbitrary(msg)
    pdelta()
    puts "arbitrary length = #{msg.length}"
  end

  def track_uses_channel(chan)
    @chan_mask = @chan_mask | (1 << chan)
  end

end

# ================================================================

run()