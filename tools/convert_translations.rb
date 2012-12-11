#!/usr/bin/ruby
# Francesco Cecconi <francesco.cecconi@gmail.com>
# convert ts2po / po2ts and create a pot file from ts
# Required: lconvert(qt4 script)

class Tools
def ts2po
    result = %x[echo; ls ../src/ts/]
    result.lines("\n") do |value|
        if (value.start_with?("nmapsi4_"))
	    tmp_clear = value.delete("\n")
            tmp_clear[".ts"] = ""
            tmp_clear["nmapsi4_"] = ""
	    tmp_clear = tmp_clear + ".po"
	    path_string_dst = "../src/ts/po/nmapsi4/"+tmp_clear
	    path_string_src = "../src/ts/"+value;
	    path_string_src = path_string_src.delete("\n")
	    path_string_dst = path_string_dst.delete("\n")
	    result = %x[echo; lconvert -of po #{path_string_src} -o #{path_string_dst}]
            puts "DEBUG::ts2po -- source file:: #{path_string_src} -- dst file:: #{path_string_dst}"
	end
    end
    buildPot
end

def po2ts
    result = %x[echo; ls ../src/ts/po/nmapsi4/]
    result.lines("\n") do |value|
        if ((value != "\n") && (!value.start_with?("nmapsi4")))
            tmp_clear = "nmapsi4_" + value.delete("\n")
            tmp_clear[".po"] = ""
            tmp_clear = tmp_clear + ".ts"
            path_string_src = "../src/ts/po/nmapsi4/"+value
            path_string_dst = "../src/ts/"+tmp_clear;
            path_string_src = path_string_src.delete("\n")
            path_string_dst = path_string_dst.delete("\n")
            result = %x[echo; lconvert -of ts #{path_string_src} -o #{path_string_dst}]
            puts "DEBUG::po2ts -- source file:: #{path_string_src} -- dst file:: #{path_string_dst}"
        end
    end
end

def buildPot(path)

    if (path != nil)
        path_string_dst = path+"nmapsi4.pot"
    else
        path_string_dst = "../src/ts/po/nmapsi4/nmapsi4.pot"
    end

    path_string_src = "../src/ts/template/nmapsi4.ts"
    result = %x[echo; lconvert -of pot #{path_string_src} -o #{path_string_dst}]
    puts "DEBUG::POT:: -- source file:: #{path_string_src} -- dst file:: #{path_string_dst}"
end

end


# MAIN
if __FILE__ == $0

    if $*.size == 0
	print "Syntax: convert_translations.rb ts2po/po2ts...\n"
    else if ARGV.first == "ts2po"
	    ts = Tools.new
	    ts.ts2po
        else if ARGV.first == "buildPot"
            ts = Tools.new
            if ARGV[1] != nil
                ts.buildPot(ARGV[1])
            else
                ts.buildPot(nil)
            end
            else if ARGV.first == "po2ts"
                ts = Tools.new
                ts.po2ts
                else
                    print "Syntax: convert_translations.rb ts2po/po2ts...\n"
                end
            end
        end
    end

end
