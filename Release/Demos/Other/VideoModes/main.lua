require 'lib.cheetah'
require 'lib.lquery.init'
cheetah.init('Screen modes' ,'1x1 noframe')
table.print(cheetah.getModes())