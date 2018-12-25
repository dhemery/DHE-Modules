module DHE
  class SvgFile
    attr_reader :has_text, :path, :content

    def initialize(path:, content:, has_text: false)
      @path = path.sub_ext('.svg')
      @content = content
      @has_text = has_text
    end

    def write(dir)
      file_path = dir / path
      file_path.parent.mkpath
      file_path.open('w') {|file| file.write @content}
    end
  end
end
