mkdir thumb
# multi file processor of Imagemagik
# create thumbnails
mogrify -resize 200x150 -background black -gravity center -extent 200x150 -format ".thumb.JPG" -quality 85 -path thumb '*JPG'
# create lower resolution images
mogrify -resize 1600x1200 -background black -gravity center -extent 1600x1200 -format ".JPG" -quality 85 -path smaller2 '*JPG'