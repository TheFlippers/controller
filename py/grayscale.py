from PIL import Image
import sys, time

THRESHOLD = 382
WHITE = (255, 255, 255, 255)
BLACK = (0, 0, 0, 255)

def file_prefix(filename):
	return filename.split('.')[0]

def get_pixels(pixels, start, stop):
	
	for x in range(start[0], stop[0]):
		for y in range(start[1], stop[1]):

			yield pixels[x, y]

def to_gray(image_name):
	
	image = Image.open(image_name)
	width, height = image.size
	pixels = image.load()

#	start = hwcounter.count()

	for y in range(height):
		for x in range(width):
			
			gray = int(sum(pixels[x,y][:3]) / 3)
			pixels[x,y] = (gray, gray, gray, 255)

#	elapsed = hwcounter.count_end() - start
#	print('To Gray Cylces: {0} (average: {1:0.4f})'.format(elapsed, elapsed / (width * height)))

	image.save(file_prefix(image_name) + '_gray.png')

def sierra_lite(image_name):

	image = Image.open(image_name)
	width, height = image.size
	pixels = image.load()

	row_error = [0 for x in range(width)]
	nxt_error = [0 for x in range(width)]

#	start = hwcounter.count()

	for y in range(height):	
		for x in range(width):
			
			gray = int(sum(pixels[x,y][:3]) / 3)
			curr_error = row_error[x]

			pixel_total = gray + curr_error
			
			if pixel_total > 127:
				pixels[x,y] = WHITE
				curr_error = pixel_total - 255
			else:
				pixels[x,y] = BLACK
				curr_error = pixel_total

			if x < width - 1:
				row_error[x + 1] += curr_error >> 1
			if y < height - 1:
				nxt_error[x] += curr_error >> 2
				if x > 0:
					nxt_error[x - 1] += curr_error >> 2

		row_error = nxt_error
		nxt_error = [0 for x in range(width)]

#	elapsed = hwcounter.count_end() - start
#	print('Sierra Lite Cylces: {0} (average: {1:0.4f})'.format(elapsed, elapsed / (width * height)))

	image.save(file_prefix(image_name) + '_sierra.png')

def to_tiles(image_name, screen_size):

	image = Image.open(image_name)
	width, height = image.size
	pixels = image.load()
	
	cell_width = int(width / screen_size[0])
	cell_height = int(height / screen_size[1])

#	start = hwcounter.count()

	for i in range(screen_size[0]):
		for j in range(screen_size[1]):

			offset_x = i * cell_width
			offset_y = j * cell_height
			cell_count = 0

			for x in range(offset_x, offset_x + cell_width):
				for y in range(offset_y, offset_y + cell_height):
					cell_count += sum(pixels[x,y][:3])

			if cell_count > THRESHOLD * cell_width * cell_height:
				output = (255, 255, 255, 255)
			else:
				output = (0, 0, 0, 255)

			for x in range(offset_x, offset_x + cell_width):
				for y in range(offset_y, offset_y + cell_height):
					pixels[x,y] = output

#	elapsed = hwcounter.count_end() - start
#	print('To Tiles Cylces: {0} (average: {1:0.4f})'.format(elapsed, elapsed / (width * height)))

	image.save(file_prefix(image_name) + '_tiles.png')

if __name__ == '__main__':
	if len(sys.argv) != 2:
		print('Usage: python3 grayscale.py <image_file>')
		exit(0)

	filename = sys.argv[1]

	start = time.time()
	to_gray(filename)
	end = time.time()
	print('To Gray Duration: {:0.4f} secs'.format(end - start))

	start = time.time()
	sierra_lite(filename)
	end = time.time()
	print('Sierra Lite Duration: {:0.4f} secs'.format(end - start))

	start = time.time()
	to_tiles(filename, (7, 7))
	end = time.time()
	print('To Tiles Duration: {:0.4f} secs'.format(end - start))
