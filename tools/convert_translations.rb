#!/usr/bin/env ruby
# Francesco Cecconi francesco dot cecconi at gmail dot com
# convert ts template file to po file
# Required: ts2po tool

class Tools
def ts2PoNmapsi4
    @tmp = %x[echo; ls ../nmapsi4/ts/]
    for value in @tmp
	if (value_str = value.eql?("po\n")) == false && (value_str2 = value.eql?("\n")) == false
	    tmp_clear = value.delete("\n")
	    tmp_clear = value.chop.chop.chop.chop
	    tmp_clear = tmp_clear + ".po"
	    path_string_dst = "../nmapsi4/ts/po/"+tmp_clear;
	    path_string_src = "../nmapsi4/ts/"+value;
	    path_string_src = path_string_src.delete("\n")
	    path_string_dst = path_string_dst.delete("\n")
	    result = %x[echo; ts2po #{path_string_src} #{path_string_dst};]
	    puts "Result: #{result}"
	end
    end
end
def ts2PoNmapsi4_logr
    @tmp = %x[echo; ls ../nmapsi4-logr/ts/]
    for value in @tmp
	if (value_str = value.eql?("po\n")) == false && (value_str2 = value.eql?("\n")) == false
	    tmp_clear = value.delete("\n")
	    tmp_clear = value.chop.chop.chop.chop
	    tmp_clear = tmp_clear + ".po"
	    path_string_dst = "../nmapsi4-logr/ts/po/"+tmp_clear;
	    path_string_src = "../nmapsi4-logr/ts/"+value;
	    path_string_src = path_string_src.delete("\n")
	    path_string_dst = path_string_dst.delete("\n")
	    result = %x[echo; ts2po #{path_string_src} #{path_string_dst};]
	    puts "Result: #{result}"
	end
    end
end
end


# MAIN
if __FILE__ == $0

    if $*.size == 0
	print "Syntax: update.rb convert...\n"
    else if ARGV.first == "convert"
	    ts = Tools.new
	    ts.ts2PoNmapsi4
	    #ts.ts2PoNmapsi4_logr
	else if ARGV.first == "revert"
	    ts = Tools.new
	    # revert metod
             end
         end
    end
end

