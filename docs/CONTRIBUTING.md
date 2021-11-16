![Kodi Logo](resources/banner_slim.png)

# Contributing to Kodi Home Theater
Looking to **contribute code** to Kodi Home Theater? Great! You landed on the right place and we appreciate you for taking the time. Can't code for your life? No shame in that. There's a lot more to the Kodi project than just code. Check out **[other ways](../README.md#how-to-contribute)** to contribute.

First, a few words of caution: Kodi's huge codebase spans over 15 years of development, hundreds of volunteer developers from all walks of life and countless, sleepless hours. Please remember that somewhere in between your first *WTH?* and before you start ranting or splitting hairs.

Also, always keep in mind that for many of us English is not our native language. What might come across as discourteous is probably an unintended side effect of the language barrier. Usage of emoticons is highly encouraged. They are great to convey, er, emotions and state of mind.

## Getting Started
Before you start coding we advise you to build Kodi's master branch. This will save you and us precious time by making sure a proper build environment is set up and all dependencies are met. For your convenience, **[build guides](README.md)** are available for most common platforms. A **[git guide](GIT-FU.md)** is also available, streamlined to Kodi's development workflow. Unless you're a *git ninja*, please read it carefully.  
If you are a *git ninja*, we feel happy and sad for you at the same time. Your soul might be lost to the dark side already but worry not, we have cookies!

## Reviews
Reviews are a great way to get familiar with the codebase. If you would like to start with reviewing just use **[this handy github filter](https://github.com/xbmc/xbmc/pulls?q=is%3Apr+is%3Aopen+review%3Anone)** to see PRs that are in need of one (we have plenty!).

## Pull request guidelines
* **Create topic branches**. Never ever open a pull request from your master branch. **Ever!**
* **Code against master branch**. Unless release specific, all Kodi development occurs in the `master` branch. If it needs backporting it can be done after it hits master.
* **One topic branch per change**. Found something that needs fixing but is unrelated to the current work? Create a new topic branch and open another *pull request*.
* **Use a coherent commit history**. Make sure each individual commit in your *pull request* is meaningful and organized in logical chunks. Tidy up and squash commits before submitting.
* **Be descriptive, but concise**. The *pull request* and commits should have a useful title and follow the `[component(s)] Short description...` format. No, `[README.md] Update` is **not** descriptive enough. The *pull request* description should only contain information relevant to the change.
* **Be patient, be kind, rewind**. We're all volunteers with busy lives but usually very responsive in cataloging *pull requests* and pinging the relevant maintainer(s). Sometimes though, things go unnoticed for longer than usual. Probably due to beer deprivation... or lack thereof. Feel free to nudge us if we forget.

## Pull request - Merge guidelines
* **Jenkins builds green light on all platforms**. Make sure the Jenkins builds have a green light for each platform, anyway some compile warnings could be reported in builds, in some cases it is opportune to analyze them to not introduce new warnings.
* **Wait for approval**. Keyword be patient, infinitely patient, one approval is mandatory to be allowed to merge, 
it is normal to have to wait for others to review and approve your work, you can think in terms of weeks not days, the time and number of approvals can depend on factors like:
The number of changes applied, a huge PR could take more time to be reviewed, if appropriate you can split it in more PR's; Which Kodi areas it covers,
if your PR cover more Kodi areas could mean that more than one approval is needed, often Kodi parts are tangled and what looks perfect to you may have unwanted side effects, having others approve your work is always worthwhile.
In some cases you may be asked to make more changes to improve your PR, which may result in differences of opinion we look to discuss and resolve via consensus then
if you feel demotivated or furious, be patient no one here wants you to feel like that, we do all have good intent, but we just have an odd way of expressing it.
If your PR has been stalled for a long time you can let us know, we have a lot of work and the team is not very big we try to give our best, but we also have a real life.
* **Provide a test build**. In some cases, a test build is a powerful way to speed up the review process, the team includes members that can't build Kodi but will test things. A test build can also be used to allow users to test changes themselves and provide more feedback. 
You can request a test build from your PR which will be automatically built and uploaded to our mirrors automatically by Jenkins.

## Coding guidelines
* **Follow our [code guidelines](CODE_GUIDELINES.md)**. New code should follow those guidelines even if existing code doesn't. If you're up to it, improve existing code and submit it in separate commits.
* **Document the code**. You know you're brilliant, but maybe you'd like to understand what you did a year from now. Or, code God forbid, help others... Nope, `// Magic. Do not touch!` does not count as documentation.
* **Separate code from cosmetics**. Code and cosmetic changes should be in separate commits. It's already hard to review code without added noise. Of course, deleting a **few** trailing spaces does not warrant a separate commit. Use your judgement.
* **Test your changes**. Kodi's **[continuous integration system](http://jenkins.kodi.tv/)** builds every PR automatically. Nonetheless, you're expected to test the code on your development platform.

## Updating your PR
Making a *pull request* adhere to the standards above can be difficult. If the maintainers notice anything that they'd like changed, they'll ask you to edit your *pull request* before it gets merged. **There's no need to open a new *pull request*, just edit the existing one**. If you're not sure how to do that, our **[git guide](GIT-FU.md)** provides a step by step guide. If you're still not sure, ask us for guidance. We're all fairly proficient with *git* and happy to be of assistance.

<a href="https://github.com/xbmc/xbmc"><img src="https://forthebadge.com/images/badges/made-with-c-plus-plus.svg" height="25"></a>
<a href="https://github.com/xbmc/xbmc"><img src="https://forthebadge.com/images/badges/contains-technical-debt.svg" height="25"></a>

