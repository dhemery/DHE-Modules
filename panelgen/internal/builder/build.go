package builder

import (
	"bytes"
	"encoding/xml"
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"dhemery.com/DHE-Modules/panelgen/internal/panel"
	"dhemery.com/DHE-Modules/panelgen/internal/svg"
)

func Generate() error {
	contents := map[string][]byte{}

	for moduleSlug, p := range buildPanels() {
		imagePath := filepath.Join(imageDir, moduleSlug+".svg")
		imageBytes, err := encode(p.ImageSvg())
		if err != nil {
			return err
		}
		contents[imagePath] = imageBytes
		for frameSlug, frameSvg := range p.FrameSvgs() {
			framePath := filepath.Join(frameDir, p.FrameDir, frameSlug+".svg")
			frameBytes, err := encode(frameSvg)
			if err != nil {
				return err
			}
			contents[framePath] = frameBytes
		}
	}

	for path, content := range contents {
		if !outdated(path, content) {
			continue
		}
		if err := write(path, content); err != nil {
			return err
		}
	}
	return nil
}

func List() {
	pathSet := make(map[string]bool)
	for moduleSlug, p := range buildPanels() {
		imagePath := filepath.Join(imageDir, moduleSlug+".svg")
		pathSet[imagePath] = true
		for _, c := range p.Controls {
			for frameSlug := range c.Frames {
				framePath := filepath.Join(frameDir, p.FrameDir, frameSlug+".svg")
				pathSet[framePath] = true
			}
		}
	}
	paths := []string{}
	for path := range pathSet {
		paths = append(paths, path)
	}
	sort.Sort(sort.StringSlice(paths))
	fmt.Println(strings.Join(paths, "\n"))
}

var builtPanels map[string]*panel.Panel

func buildPanels() map[string]*panel.Panel {
	if builtPanels == nil {
		builtPanels = map[string]*panel.Panel{}
		for slug, buildPanel := range panel.Builders() {
			builtPanels[slug] = buildPanel()
		}
	}
	return builtPanels
}

const (
	buildDir = "_build"
)

var (
	frameDir = filepath.Join(buildDir, "frames")
	imageDir = filepath.Join(buildDir, "images")
)

func encode(s svg.Svg) ([]byte, error) {
	b := &bytes.Buffer{}
	b.Write([]byte(xml.Header))
	e := xml.NewEncoder(b)
	e.Indent("", "    ")
	err := e.Encode(s)
	return b.Bytes(), err

}

func write(path string, content []byte) error {
	if err := os.MkdirAll(filepath.Dir(path), 0755); err != nil {
		return err
	}
	w, err := os.Create(path)
	if err != nil {
		return err
	}
	_, err = w.Write(content)
	return err
}

func outdated(path string, content []byte) bool {
	b, err := os.ReadFile(path)
	if err != nil {
		return true
	}
	return bytes.Compare(b, content) != 0
}
