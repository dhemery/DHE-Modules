package panel

func Builders() map[string]buildFunc {
	return builders
}

type buildFunc func() *Panel

var builders map[string]buildFunc

func registerBuilder(slug string, fn buildFunc) {
	if builders == nil {
		builders = map[string]buildFunc{}
	}
	builders[slug] = fn
}
