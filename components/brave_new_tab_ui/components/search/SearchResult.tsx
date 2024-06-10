// Copyright (c) 2024 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.
import Flex from '$web-common/Flex';
import { getLocale } from '$web-common/locale';
import Icon from '@brave/leo/react/icon';
import { color, font, gradient, icon, radius, spacing } from '@brave/leo/tokens/css/variables';
import { mojoString16ToString } from 'chrome://resources/js/mojo_type_util.js';
import { useUnpaddedImageUrl } from '../../../brave_news/browser/resources/shared/useUnpaddedImageUrl';
import { AutocompleteMatch } from 'gen/ui/webui/resources/cr_components/searchbox/searchbox.mojom.m';
import * as React from 'react';
import styled from 'styled-components';
import { omniboxController } from './SearchContext';

interface Props {
  match: AutocompleteMatch
  line: number
  selected: boolean
}

const Container = styled.a`
  padding: ${spacing.s} ${spacing.m};
  border-radius: ${radius.m};

  display: flex;
  flex-direction: row;
  align-items: center;
  gap: ${spacing.l};

  text-decoration: none;

  &[aria-selected=true], &:hover {
    background: rgba(255, 255, 255, 0.1);
  }
`

const IconContainer = styled.div`
  border-radius: ${radius.m};
  width: 32px;
  height: 32px;

  display: flex;
  align-items: center;
  justify-content: center;

  flex-shrink: 0;

  > span, > img {
    width: 20px;
    height: 20px;
  }
`

const FavIcon = styled.span<{ url: string }>`
  background: rgba(255, 255, 255, 0.5);
  mask-image: url(${p => p.url});
  mask-size: contain;
`

const Content = styled.span`
  font: ${font.large.regular};
  line-height: 24px;
  color: ${color.text.secondary};
`

const Description = styled.span`
  font: ${font.small.regular};
  line-height: 18px;
  color: rgba(255,255,255,0.7);
`

const Hint = styled.span`
  color: ${color.text.interactive};
`

const LeoIcon = styled(Icon)`
  --leo-icon-size: ${icon.m};

  color: ${color.white};
  background: ${gradient.iconsActive};
  border-radius: ${radius.m};
  padding: ${spacing.s};
`

const Divider = styled.hr`
  margin: 2px -8px;
  opacity: 0.1;
`

const hide = { opacity: 0 }
const show = { opacity: 1 }
function RichImage({ url }: { url: string }) {
  const [loaded, setLoaded] = React.useState(false)
  const iconUrl = useUnpaddedImageUrl(url, () => setLoaded(true))
  return <img src={iconUrl} style={loaded ? show : hide} />
}
function Image({ match, isAskLeo }: { match: AutocompleteMatch, isAskLeo: boolean }) {
  if (isAskLeo) return <LeoIcon name='product-brave-leo' />

  const isGeneric = !match.imageUrl
  return isGeneric
    ? <FavIcon url={match.iconUrl} />
    : <RichImage url={match.imageUrl} />
}

export default function SearchResult({ match, line, selected }: Props) {
  const contents = mojoString16ToString(match.swapContentsAndDescription ? match.description : match.contents)
  const description = mojoString16ToString(match.swapContentsAndDescription ? match.contents : match.description)
  const isAskLeo = description === getLocale('searchAskLeo')

  const hint = description && match.destinationUrl.url
    ? description
    : ''

  const result = <Container href={match.destinationUrl.url} aria-selected={selected} onClick={e => {
    e.preventDefault()
    omniboxController.openAutocompleteMatch(line, match.destinationUrl, true, e.button, e.altKey, e.ctrlKey, e.metaKey, e.shiftKey)
  }}>
    <IconContainer>
      <Image key={match.imageUrl ?? match.iconUrl} match={match} isAskLeo={isAskLeo} />
    </IconContainer>
    <Flex direction='column'>
      <Content>{contents}<Hint>{hint ? ` - ${hint}` : ''}</Hint></Content>
      {description && description !== hint && <Description>{description}</Description>}
    </Flex>
  </Container>

  return isAskLeo
    ? <>
      <Divider />
      {result}
    </>
    : result
}
