module.exports = {
  siteMetadata: {
    title: `Tech resume`,
    description: `Create and mantain your resume in markdown syntax. Kick off your career by using curriculum vitae templates.`,
    author: `@EliaECoyote`,
  },
  plugins: [
    // `gatsby-plugin-react-helmet`,
    // {
    //   resolve: `gatsby-source-filesystem`,
    //   options: {
    //     name: `images`,
    //     path: `${__dirname}/src/images`,
    //   },
    // },
  
    // ignore all pages declared in src/pages
    {
      resolve: `gatsby-plugin-page-creator`,
      options: {
        path: `${__dirname}/src/pages`,
        ignore: {
          patterns: [`**/*.(js|ts)?(x)`],
        },
      },
    },
    // use bucklescript-created pages inside the
    // **lib** compilation folder
    {
      resolve: `gatsby-plugin-page-creator`,
      options: {
        path: `${__dirname}/lib/js/src/pages`,
      },
    },
    {
      resolve: `gatsby-plugin-manifest`,
      options: {
        name: `Tech Resume`,
        short_name: `Resume`,
        start_url: `/`,
        background_color: `#282c33`,
        theme_color: `#282c33`,
        display: `standalone`,
        icon: `src/images/app-icon.png`,
        icon_options: {
          purpose: `maskable`,
        },
      },
    },
    // this (optional) plugin enables Progressive Web App + Offline functionality
    // To learn more, visit: https://gatsby.dev/offline
    // `gatsby-plugin-offline`,
  ],
}
